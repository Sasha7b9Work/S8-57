#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "AD9286.h"
#include "Hardware/Timer.h"
#include <stm32f4xx.h>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PIN_SCK     GPIO_PIN_10
#define PORT_SCK    GPIOB

#define PIN_DAT     GPIO_PIN_3
#define PORT_DAT    GPIOC

#define PIN_CS      GPIO_PIN_11
#define PORT_CS     GPIOE

#define SCK         PORT_SCK, PIN_SCK
#define DAT         PORT_DAT, PIN_DAT
#define CS          PORT_CS,  PIN_CS


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AD9286::Init()
{
    /*
        Для чипселекта используется вывод
        А вот для тактов и данных используются те же, выводы, которые в дальнейшем управляют микросхемой EEPROM AT25160

        CS  - 64 - PE11
        DAT - 29 - PC3
        SCK - 69 - PB10
    */
   
    GPIO_InitTypeDef isGPI0 =                   // Инициализация CS
    {
        PIN_CS,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };
    HAL_GPIO_Init(PORT_CS, &isGPI0);

    isGPI0.Pin = PIN_DAT;                       // Инициализация DAT
    HAL_GPIO_Init(PORT_DAT, &isGPI0);
    
    isGPI0.Pin = PIN_SCK;                       // Инициализация SCK
    HAL_GPIO_Init(PORT_SCK, &isGPI0);

    SetPin(CS);
    ResetPin(DAT);
    ResetPin(SCK);

    Tune();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AD9286::Tune()
{
    AD9286::WriteByte(0x09, 0x09);
    /// \todo Это должно быть в функции записи
    AD9286::WriteByte(0xff, 0x01);   // Пишем бит подтверждения.
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AD9286::ConfigToRead()
{
    /*
    isGPIO.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOE, &isGPIO);
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AD9286::ConfigToWrite()
{
    /*
    isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOE, &isGPIO);
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AD9286::WriteByte(uint8 address, uint8 byte)
{
    ResetPin(CS);

    uint value = (uint)((address << 8) + byte);

    for (int i = 23; i >= 0; --i)
    {
        if (_GET_BIT(value, i))
        {
            SetPin(DAT);
        }
        else
        {
            ResetPin(DAT);
        }
        PAUSE_ON_TICKS(100);

        SetPin(SCK);
        PAUSE_ON_TICKS(100);

        ResetPin(SCK);
    }

    SetPin(CS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 AD9286::ReadByte(uint8)
{
    return 0;

    /*
    uint16 value = (uint16)(0x8000 + address);

    ResetPin(SPI4_CS);

    for (int i = 15; i >= 0; --i)
    {
        if (_GET_BIT(value, i))
        {
            SetPin(SPI4_DAT);
        }
        else
        {
            ResetPin(SPI4_DAT);
        }
        PAUSE_ON_TICKS(100);

        SetPin(SPI4_SCK);
        PAUSE_ON_TICKS(100);

        ResetPin(SPI4_SCK);
    }

    SetPin(SPI4_CS);

    uint8 retValue = 0;
    
    ConfigToRead();

    ResetPin(SPI4_CS);

    for (int i = 7; i >= 0; --i)
    {
        SetPin(SPI4_SCK);
        int bit = ReadPin(SPI4_DAT);
        if (bit)
        {
            _SET_BIT_VALUE(retValue, i, bit);
        }
        ResetPin(SPI4_SCK);
        PAUSE_ON_TICKS(100);
    }

    SetPin(SPI4_CS);

    ConfigToWrite();
    
    return retValue;
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AD9286::SetPin(GPIO_TypeDef *_gpio, uint16 pin)
{
    HAL_GPIO_WritePin(_gpio, pin, GPIO_PIN_SET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AD9286::ResetPin(GPIO_TypeDef *_gpio, uint16 pin)
{
    HAL_GPIO_WritePin(_gpio, pin, GPIO_PIN_RESET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int AD9286::ReadPin(uint pin)
{
    return HAL_GPIO_ReadPin(GPIOE, (uint16)pin);
}
