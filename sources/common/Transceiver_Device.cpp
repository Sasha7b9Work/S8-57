#include "defines.h"
#include "log.h"
#include "DataBus.h"
#include "Transceiver.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>
#include "Keyboard/DecoderDevice.h"


#define PORT_MODE0  GPIOA
#define PIN_MODE0   GPIO_PIN_7
#define MODE0       PORT_MODE0, PIN_MODE0

#define PORT_MODE1  GPIOC
#define PIN_MODE1   GPIO_PIN_4
#define MODE1       PORT_MODE1, PIN_MODE1

#define PORT_READY  GPIOG
#define PIN_READY   GPIO_PIN_12
#define READY       PORT_READY, PIN_READY

#define PORT_FL0    GPIOD
#define PIN_FL0     GPIO_PIN_5
#define FL0         PORT_FL0, PIN_FL0


namespace Transceiver
{

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

    namespace Receiver
    {
        /// Инициализация FL0 на чтение
        void Init_FL0_IN();
        /// Инициализировать пины для приёма из панели
        void InitPinsReceive();
        /// Возвращает состояние FL0
        State::E State_FL0();
        /// Считывает байт данных с ШД
        uint8 ReadData();
    }
}


//                                      D0           D1           D2          D3          D4          D5          D6          D7
static const GPIO_TypeDef *ports[] = { GPIOD,       GPIOD,       GPIOD,      GPIOD,      GPIOE,      GPIOE,      GPIOE,      GPIOE };
static const uint16 pins[]         = { GPIO_PIN_14, GPIO_PIN_15, GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_7, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10 };


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Init()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_MODE0;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(PORT_MODE0, &gpio);   // MODE0 - используется для выбора режима

    gpio.Pin = PIN_MODE1;
    HAL_GPIO_Init(PORT_MODE1, &gpio);   // MODE1 - используется для выбора режима

    gpio.Pin = PIN_READY;                 
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(PORT_READY, &gpio);   // READY - используется для чтения подтверждения из панели

    Set_MODE(Mode::Disabled);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Init_FL0_IN()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = PIN_FL0;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(PORT_FL0, &gpio);     // Будем на этом выводе узнавать, есть ли у панели данные для передачи
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::InitPinsReceive()
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 data)
{
    Send(&data, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 *data, uint size)
{
    if (DataBus::mode != DataBus::Mode::DeviceTransmit)        // Если пины ещё не инициализированы для передачи -
    {
        DataBus::mode = DataBus::Mode::DeviceTransmit;          // инициализируем
    
        /* Настроим пины 14, 15, 0, 1 на запись D0, D1, D2, D3 */
        /* Устанавливаем для этих пинов GPIO_MODE_OUTPUT_PP. */
        GPIOD->MODER &= 0x0ffffff0;
        GPIOD->MODER |= 0x50000005;
        /* Настроим пины 7, 8, 9, 10 на запись D4, D5, D6, D7 */
        /* Устанавливаем для этих пинов GPIO_MODE_OUTPUT_PP. */
        GPIOE->MODER &= 0xffc03fff;
        GPIOE->MODER |= 0x00154000;
    }

    for (uint i = 0; i < size; i++)
    {
        uint8 d = data[i];

        //                                                      биты 0,1                      биты 2, 3
        GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + (uint16)(((int16)d & 0x03) << 14) + (((uint16)(d & 0x0c)) >> 2);           // Записываем данные в выходные пины
        //                                                    Биты 4,5,6,7
        GPIOE->ODR = (GPIOE->ODR & 0xf87f) + (uint16)(((int16)d & 0xf0) << 3);

                                                        // Даём сигнал панели, что можно считывать данные
                                                        // Set_MODE(Mode::Send);
        PORT_MODE0->BSRR = (uint)PIN_MODE0 << 16U;      //HAL_GPIO_WritePin(MODE0, GPIO_PIN_RESET);

        PORT_MODE1->BSRR = PIN_MODE1;                   //HAL_GPIO_WritePin(MODE1, GPIO_PIN_SET);

        while (State_READY() == State::Passive) {};     // Ожидаем сигнал подтверждения

        Set_MODE(Mode::Disabled);                       // Даём признак, что подтверждение получено. Теперь панель должна убрать сигнал READY

        while (State_READY() == State::Active) {};
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Transceiver::Update()
{
    DataBus::mode = DataBus::Mode::DeviceReceive;

    Receiver::Init_FL0_IN();                    // Инициализируем FL0 на чтение

    Set_MODE(Mode::Receive);                    // Сообщаем панели, что готовы принять данные

    while (State_READY() == State::Passive) {}; // Ожидаем сигнал готовности от панели

    if (Receiver::State_FL0() == State::Passive)      // Если панель сообщает о том, что данных нет
    {
        Set_MODE(Mode::Disabled);               // То отключаем взаимодействие с панелью

        return false;                            // и выходим
    }

    Receiver::InitPinsReceive();                // Инициалазируем пины данных на приём

    uint8 data = Receiver::ReadData();          // Читаем байт
    
    Decoder::AddData(data);                     // И отправляем его на выполнение

    Set_MODE(Mode::Disabled);

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Transceiver::State::E Transceiver::State_READY()
{
    return (HAL_GPIO_ReadPin(READY) == GPIO_PIN_SET) ? State::Active : State::Passive;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Set_MODE(Mode::E mode)
{
    if (mode == Mode::Send)
    {
        HAL_GPIO_WritePin(MODE0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MODE1, GPIO_PIN_SET);
    }
    else if (mode == Mode::Receive)
    {
        HAL_GPIO_WritePin(MODE1, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MODE0, GPIO_PIN_SET);
    }
    else if (mode == Mode::Disabled)
    {
        HAL_GPIO_WritePin(MODE0, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(MODE1, GPIO_PIN_RESET);
        /// \todo С этим надо что-то делать. Непонятно, почему без задержки не работает
        //Timer::PauseOnOPS(200);
    }
    else
    {
        // здесь ничего не надо
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Transceiver::State::E Transceiver::Receiver::State_FL0()
{
    return (HAL_GPIO_ReadPin(FL0) == GPIO_PIN_SET) ? State::Active : State::Passive;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Transceiver::Receiver::ReadData()
{
    uint8 result = 0;

    for (int i = 7; i >= 0; i--)
    {
        result |= HAL_GPIO_ReadPin((GPIO_TypeDef *)ports[i], pins[i]);

        if (i != 0)
        {
            result <<= 1;
        }
    }

    return result;
}
