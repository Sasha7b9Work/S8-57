#pragma once
#include "Hardware/HAL/HAL_PINS.h"


struct HMode
{
    enum E
    {
        Analog,
        Output_PP,
        RisingIT,
        Input,
        AF_PP,
        Count
    };
};

struct HPull
{
    enum E
    {
        No,
        Down,
        Up,
        Count
    };
};

struct HSpeed
{
    enum E
    {
        Low,
        High,
        VeryHigh,
        Count
    };
};

struct HAlternate
{
    enum E
    {
        AF0_MCO,
        AF5_SPI4,
        AF7_USART3,
        AF10_OTG_FS,
        AF12_OTG_HS_FS,
        Count
    };
};

struct HAL_PIO
{
    static void Init(HPort::E port, uint pins, HMode::E mode, HPull::E pull, HSpeed::E speed = HSpeed::Low, HAlternate::E alternate = HAlternate::AF0_MCO);
    // Установка в "1"
    static void Set(HPort::E port, uint16 pin);
    // Установка в "0"
    static void Reset(HPort::E port, uint16 pin);
    static void Write(HPort::E port, uint16 pin, int state);
    static void Write(const StructPIN &pin, int state);
    static uint8 Read(HPort::E port, uint16 pin);

    static void EXTI_CLEAR_IT_BIT(uint16 pin);
};
