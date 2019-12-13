#pragma once


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


#define PORT_CS_RAM     HPort::_G, HPin::_10

#define PORT_AD9286_SCK HPort::_B, HPin::_10
#define PORT_AD9286_DAT HPort::_C, HPin::_3


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


struct HState
{
    enum E
    {
        Disabled,
        Enabled,
        Count
    };
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
        VeryHigh,
        Count
    };
};

struct HAlternate
{
    enum E
    {
        AF0_MCO,
        AF7_USART3,
        Count
    };
};

struct HAL_PIO
{
    static void Init(HPort::E port, uint pins, HMode::E mode, HPull::E pull, HSpeed::E speed = HSpeed::Low, HAlternate::E alternate = HAlternate::AF0_MCO);
    /// Установка в "1"
    static void Set(HPort::E port, uint16 pin);
    /// Установка в "0"
    static void Reset(HPort::E port, uint16 pin);
    static void Write(HPort::E port, uint16 pin, HState::E state);
    static bool Read(HPort::E port, uint16 pin);

    static void EXTI_CLEAR_IT_BIT(uint16 pin);
};
