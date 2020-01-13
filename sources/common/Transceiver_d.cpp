#include "defines.h"
#include "Decoder_d.h"
#include "Transceiver.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>

#ifdef DEVICE
#include "Osci/Osci.h"
#include "Recorder/Recorder.h"
#endif


struct PinWR
{
    static void SetActive()  { HAL_PIO::Reset(PIN_P_WR); }
    static void SetPassive() { HAL_PIO::Set(PIN_P_WR); }
};


struct PinRD
{
    static void SetAcitve();
    static void SetPassive();
};


struct PinCS
{
    static void SetActive()  { HAL_PIO::Reset(PIN_P_CS); }
    static void SetPassive() { HAL_PIO::Set(PIN_P_CS); }
};


// Пин занятости панели
struct PinBusy
{
    static bool IsActive()  { return HAL_PIO::Read(PIN_P_BUSY) == 0; }
    static bool IsPassive() { return HAL_PIO::Read(PIN_P_BUSY) == 1; }
};


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


struct Receiver
{
    /// Считывает байт данных с ШД
    static uint8 ReadData();
};


void Transceiver::Init()
{
    HAL_PIO::Init(PIN_P_BUSY, HMode::Input, HPull::Up);
    HAL_PIO::Init(PIN_P_DATA_READY, HMode::Input, HPull::Up);
}


void Transceiver::Send(uint8 data)
{
    Send(&data, 1);
}


static void SendByte(uint8 byte)
{
    // Устанавливаем данные на шине данных

    //                                                                                биты 0,1                                    биты 2,3
    GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + static_cast<uint16>((static_cast<int16>(byte) & 0x03) << 14) + ((static_cast<uint16>(byte & 0x0c)) >> 2);  // Записываем данные в выходные пины
    //                                                                             Биты 4,5,6,7
    GPIOE->ODR = (GPIOE->ODR & 0xf87f) + static_cast<uint16>((static_cast<int16>(byte) & 0xf0) << 3);

    PinWR::SetActive();             // Устанавливаем флаг записи

    while(PinBusy::IsActive()) {};  // И ждём, пока панель выставит флаг готовности к взаимодействию

    PinCS::SetActive();             // Устанавливаем чипселект

    while(PinBusy::IsPassive()) {}; // Ждём, пока панель опять перейдёт в занятое состояние - считает даные

    PinCS::SetPassive();            // Заканчиваем цикл обмена
    PinWR::SetPassive();
}


void Transceiver::Send(const uint8 *data, uint size)
{
    inInteraction = true;

    if (DataBusMode::state != DataBusMode::DeviceTransmit)        // Если пины ещё не инициализированы для передачи -
    {
        DataBusMode::state = DataBusMode::DeviceTransmit;          // инициализируем
        
        GPIOD->MODER &= 0x0ffffff0U;        // Настроим пины 14, 15, 0, 1 на запись D0, D1, D2, D3
        GPIOD->MODER |= 0x50000005U;        // Устанавливаем для этих пинов GPIO_MODE_OUTPUT_PP
        
        GPIOE->MODER &= 0xffc03fffU;        // Настроим пины 7, 8, 9, 10 на запись D4, D5, D6, D7
        GPIOE->MODER |= 0x00154000U;        // Устанавливаем для этих пинов GPIO_MODE_OUTPUT_PP

        HAL_PIO::InitOutput(PIN_P_CS, HPull::Down, 1);
        HAL_PIO::InitOutput(PIN_P_WR, HPull::Down, 1);
        HAL_PIO::InitOutput(PIN_P_RD, HPull::Down, 1);
    }


    for (uint i = 0; i < size; i++)
    {
        SendByte(*data++);
    }

    inInteraction = false;
}


bool Transceiver::Receive()
{
    return false;
}


uint8 Receiver::ReadData()
{
    uint8 result = HAL_PIO::Read(PIN_P_D7);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D6);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D5);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D4);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D3);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D2);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D1);
    result <<= 1;
    result |= HAL_PIO::Read(PIN_P_D0);

    return result;
}
