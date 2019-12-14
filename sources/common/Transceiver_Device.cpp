#include "defines.h"
#include "common/Transceiver.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Keyboard/DecoderDevice.h"
#include <stm32f4xx_hal.h>

#ifdef DEVICE
#include "Osci/Osci.h"
#include "Recorder/Recorder.h"
#endif


struct Mode
{
    enum E
    {
        Disabled,   ///< Обмен между устройствами не идёт
        Send,       ///< Передача данных в панель
        Receive,    ///< Приём данных от панели
        Forbidden   ///< Недопустимый режим
    };
};

struct State
{
    enum E
    {
        Passive,
        Active
    };
};


/// Установка режима работы
void Set_MODE(Mode::E mode);

State::E State_READY();

struct Receiver
{
    /// Инициализация FL0 на чтение
    static void Init_FL0_IN();
    /// Инициализировать пины для приёма из панели
    static void InitPinsReceive();
    /// Возвращает состояние FL0
    static State::E State_FL0();
    /// Считывает байт данных с ШД
    static uint8 ReadData();
};


//                                      D0           D1           D2          D3          D4          D5          D6          D7
static const GPIO_TypeDef *ports[] = { GPIOD,       GPIOD,       GPIOD,      GPIOD,      GPIOE,      GPIOE,      GPIOE,      GPIOE };
static const uint16 pins[]         = { GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10 };



void Transceiver::Init()
{
    HAL_PIO::Init(PIN_MODE0, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_MODE1, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_READY, HMode::Input, HPull::Down);    // используется для чтения подтверждения из панели

    Set_MODE(Mode::Disabled);
}


void Receiver::Init_FL0_IN()
{
    HAL_PIO::Init(PIN_FL0, HMode::Input, HPull::Down);  // Будем на этом выводе узнавать, есть ли у панели данные для передачи
}


void Receiver::InitPinsReceive()
{
    GPIO_InitTypeDef gpio;

    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;

    gpio.Pin = GPIO_PIN_0  |           // D2
               GPIO_PIN_1  |           // D3
               GPIO_PIN_14 |           // D0
               GPIO_PIN_15;            // D1
    HAL_GPIO_Init(GPIOD, &gpio);

    gpio.Pin = GPIO_PIN_7 |            // D4
               GPIO_PIN_8 |            // D5
               GPIO_PIN_9 |            // D6
               GPIO_PIN_10;            // D7
    HAL_GPIO_Init(GPIOE, &gpio);
}


void Transceiver::Send(uint8 data)
{
    Send(&data, 1);
}


void Transceiver::Send(const uint8 *data, uint size)
{
#ifdef DEVICE
    Recorder::ReadPoint();
    Osci::ReadPointP2P();
#endif
    inInteraction = true;

    if (DataBusMode::state != DataBusMode::DeviceTransmit)        // Если пины ещё не инициализированы для передачи -
    {
        DataBusMode::state = DataBusMode::DeviceTransmit;          // инициализируем
        
        GPIOD->MODER &= 0x0ffffff0U;        // Настроим пины 14, 15, 0, 1 на запись D0, D1, D2, D3
        GPIOD->MODER |= 0x50000005U;        // Устанавливаем для этих пинов GPIO_MODE_OUTPUT_PP
        
        GPIOE->MODER &= 0xffc03fffU;        // Настроим пины 7, 8, 9, 10 на запись D4, D5, D6, D7
        GPIOE->MODER |= 0x00154000U;        // Устанавливаем для этих пинов GPIO_MODE_OUTPUT_PP
    }

    for (uint i = 0; i < size; i++)
    {
        uint8 d = *data++;

        //                                                                             биты 0,1                                 биты 2,3
        GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<int16>(d) & 0x03) << 14) + ((static_cast<uint16>(d & 0x0c)) >> 2);  // Записываем данные в выходные пины
        //                                                                          Биты 4,5,6,7
        GPIOE->ODR = (GPIOE->ODR & 0xf87f) + static_cast<uint16>((static_cast<int16>(d) & 0xf0) << 3);

        GPIOC->BSRR = GPIO_PIN_4;                       // Установить MODE1 в "1" - это означает, что M0M1 == 01 и устройство ждёт подверждения от панели о принятых данных

        while (!(GPIOG->IDR & GPIO_PIN_12)) {};      // Ожидаем сигнал подтверждения - "1" на READY будет означать, что панель приняла данные //-V712

        GPIOC->BSRR = (uint)GPIO_PIN_4 << 16U;          // Установить MODE1 в "0" - это означает, что устройство в состоянии Disable

        while (GPIOG->IDR & GPIO_PIN_12) {};         // Ожидаем, когда уровень на READY станет раным "0". //-V712
    }

    inInteraction = false;

#ifdef DEVICE
    Recorder::ReadPoint();
    Osci::ReadPointP2P();
#endif
}


bool Transceiver::Receive()
{
#ifdef DEVICE
    Osci::ReadPointP2P();
#endif

    inInteraction = true;

    DataBusMode::state = DataBusMode::DeviceReceive;

    Receiver::Init_FL0_IN();                        // Инициализируем FL0 на чтение

    Set_MODE(Mode::Receive);                        // Сообщаем панели, что готовы принять данные

    while (State_READY() == State::Passive)         // Ожидаем сигнал готовности от панели
    {
#ifdef DEVICE
        Osci::ReadPointP2P();
#endif
    };     

    if (Receiver::State_FL0() == State::Passive)    // Если панель сообщает о том, что данных нет
    {
        Set_MODE(Mode::Disabled);                   // То отключаем взаимодействие с панелью

        return false;                               // и выходим
    }

    Receiver::InitPinsReceive();                    // Инициалазируем пины данных на приём

    uint8 data = Receiver::ReadData();              // Читаем байт
    
    Decoder::AddData(data);                         // И отправляем его на выполнение

    Set_MODE(Mode::Disabled);

    inInteraction = false;

    return true;
}


State::E State_READY()
{
    return  HAL_PIO::Read(PIN_READY) ? State::Active : State::Passive;
}


void Set_MODE(Mode::E mode)
{
    if (mode == Mode::Send)
    {
        HAL_PIO::Reset(PIN_MODE0);
        HAL_PIO::Set(PIN_MODE1);
    }
    else if (mode == Mode::Receive)
    {
        HAL_PIO::Reset(PIN_MODE1);
        HAL_PIO::Set(PIN_MODE0);
    }
    else if (mode == Mode::Disabled)
    {
        HAL_PIO::Reset(PIN_MODE0);
        HAL_PIO::Reset(PIN_MODE1);
        /// \todo С этим надо что-то делать. Непонятно, почему без задержки не работает
        //Timer::PauseOnOPS(200);
    }
    else
    {
        // здесь ничего не надо
    }
}


State::E Receiver::State_FL0()
{
    return HAL_PIO::Read(PIN_FL0) ? State::Active : State::Passive;
}


uint8 Receiver::ReadData()
{
    uint8 result = 0;

    for (int i = 7; i >= 0; i--)
    {
        result |= HAL_GPIO_ReadPin(const_cast<GPIO_TypeDef *>(ports[i]), pins[i]);

        if (i != 0)
        {
            result <<= 1;
        }
    }

    return result;
}
