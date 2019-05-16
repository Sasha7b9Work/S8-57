#include "defines.h"
#include "Transceiver.h"
#include "Utils/DecoderPanel.h"


namespace Transceiver
{
    /// /// Эту функцию нужно вызывать всякий раз при инициализации пинов на приём или передачу.
    void(*CallbackOnInitPins)();

    namespace Transmitter
    {
        void Init();
        void InitPinsSend();
    }

    namespace Receiver
    {
        void Init();
        void InitPinsReceive();
        void DeInitPins();

        uint8 ReadData();
        int Read_MODE_CONFIRM();
        int Read_BYTE_SET();
        void Write_BYTE_CONFIRM(int);
        void Write_MODE_PANEL(int);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Init(void (*callbackInitPins)())
{
    CallbackOnInitPins = callbackInitPins;

    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_14;                                 // MODE_PANEL
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &gpio);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14, GPIO_PIN_SET);    // Устанавливаем признак того, что готовы к приёмму

    gpio.Pin = GPIO_PIN_15;                                 // MODE_CONFIRM
    gpio.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOC, &gpio);

    Receiver::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Init()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Init()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_13;             // Настраиваем BYTE_SET на приём
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &gpio);

    gpio.Pin = GPIO_PIN_5;              // Настраиваем BYTE_CONFIRM на вывод
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOD, &gpio);

    Write_BYTE_CONFIRM(0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::InitPinsReceive()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &gpio);

    gpio.Pin = GPIO_PIN_5;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;    // BYTE_CONFIRM на запись
    HAL_GPIO_Init(GPIOD, &gpio);

    gpio.Pin = GPIO_PIN_13;             // BYTE_SET на чтение
    gpio.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOC, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::DeInitPins()
{
    GPIO_InitTypeDef gpio;
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;   // D0...D7 на чтение
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &gpio);

    gpio.Pin = GPIO_PIN_5;              // BYTE_CONFIRM на чтение
    gpio.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOD, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 * /*data*/, uint /*size*/)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Update()
{
    if (Read_MODE_CONFIRM() == 0)
    {
        return;
    }

    InitPinsReceive();

    while (Read_MODE_CONFIRM() == 1)
    {
        while (Read_BYTE_SET() == 1) {}

        uint8 data = ReadData();

        Decoder::AddData(data);

        Write_BYTE_CONFIRM(1);
    }

    DeInitPins();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Transceiver::Receiver::Read_BYTE_SET()
{
    return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Transceiver::Receiver::Read_MODE_CONFIRM()
{
    return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Write_BYTE_CONFIRM(int data)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, (GPIO_PinState)data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Transceiver::Receiver::ReadData()
{
    int bit[8];

    bit[0] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0);
    bit[1] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1);
    bit[2] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2);
    bit[3] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3);
    bit[4] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4);
    bit[5] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5);
    bit[6] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6);
    bit[7] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7);

    uint8 result = 0;

    for (int i = 0; i < 8; i++)
    {
        result += (bit[i] << i) & 0x01;
    }

    return result;
}
