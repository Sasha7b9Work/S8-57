#include "defines.h"
#include "Transceiver.h"
#include <stm32f4xx_hal.h>


namespace Transceiver
{
    /// Эту функцию нужно вызывать всякий раз при инициализации пинов на приём или передачу.
    void(*CallbackOnInitPins)();

    namespace Transmitter
    {
        void Init();
        void InitPinsSend();
        int Read_MODE_PANEL();
        void Write_MODE_CONFIRM(int);
        void Write_BYTE_SET(int);
        void WriteData(uint8 data);
    }

    namespace Receiver
    {
        void Init();
        void InitPinsReceive();
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Init(void (*callbackInitPins)())
{
    CallbackOnInitPins = callbackInitPins;

    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_7;              // MODE_PANEL на чтение
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &gpio);

    gpio.Pin = GPIO_PIN_4;              // MODE_CONFIRM устанавливаем в "0" в предположении, что панель настроена на приём и MODE_PANEL == "1"
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOC, &gpio);

    Transmitter::Init();
    Receiver::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Init()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Init()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::InitPinsSend()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::InitPinsReceive()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 *data, uint size)
{
    while (Read_MODE_PANEL() == 0)
    {
        Receiver::Update();
    }

    InitPinsSend();

    Write_BYTE_SET(1);

    Write_BYTE_SET(0);

    for (uint i = 0; i < size; i++)
    {
        WriteData(data[i]);

        Write_BYTE_SET(0);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Transceiver::Transmitter::Read_MODE_PANEL()
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_MODE_CONFIRM(int data)
{
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, (GPIO_PinState)data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_BYTE_SET(int data)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, (GPIO_PinState)data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::WriteData(uint8)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Update()
{

}
