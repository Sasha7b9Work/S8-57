#pragma once


#define PIN_MODE0       HPort::_A, HPin::_7
#define PIN_MODE1       HPort::_C, HPin::_4


struct HPort
{
    enum E
    {
        _A,
        _B,
        _C,
        _D,
        _E,
        _F,
        _G,
        _H,
        Count
    };
};

struct HPin
{
    static uint16 _0;
    static uint16 _1;
    static uint16 _2;
    static uint16 _3;
    static uint16 _4;
    static uint16 _5;
    static uint16 _6;
    static uint16 _7;
    static uint16 _8;
    static uint16 _9;
    static uint16 _10;
    static uint16 _11;
    static uint16 _12;
    static uint16 _13;
    static uint16 _14;
    static uint16 _15;
};

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

    static void Reset(HPort::E port, uint16 pin);

    static void Set(HPort::E port, uint16 pin);
};
