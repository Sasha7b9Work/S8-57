#pragma once


#define PIN_P_BUSY       HPort::_A, HPin::_7   /*  43 P_BUSY       (I) */
#define PIN_P_DATA_READY HPort::_C, HPin::_4   /*  44 P_DATA_READY (I) */
#define PIN_P_CS         HPort::_G, HPin::_12  /* 127 CS           (0) */
#define PIN_P_WR         HPort::_D, HPin::_5   /* 119 WR           (0) */
#define PIN_P_RD         HPort::_D, HPin::_4   /* 118 RD           (0) */
#define PIN_P_D0         HPort::_D, HPin::_14
#define PIN_P_D1         HPort::_D, HPin::_15
#define PIN_P_D2         HPort::_D, HPin::_0
#define PIN_P_D3         HPort::_D, HPin::_1
#define PIN_P_D4         HPort::_E, HPin::_7
#define PIN_P_D5         HPort::_E, HPin::_8
#define PIN_P_D6         HPort::_E, HPin::_9
#define PIN_P_D7         HPort::_E, HPin::_10

#define PIN_HCD_DM       HPort::_B, HPin::_14
#define PIN_HCD_DP       HPort::_B, HPin::_15


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
    static const uint16 _0;
    static const uint16 _1;
    static const uint16 _2;
    static const uint16 _3;
    static const uint16 _4;
    static const uint16 _5;
    static const uint16 _6;
    static const uint16 _7;
    static const uint16 _8;
    static const uint16 _9;
    static const uint16 _10;
    static const uint16 _11;
    static const uint16 _12;
    static const uint16 _13;
    static const uint16 _14;
    static const uint16 _15;
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

    static void InitOutput(HPort::E port, uint16 pin, HPull::E pull, int state);

    static void Reset(HPort::E port, uint16 pin);

    static void Set(HPort::E port, uint16 pin);

    static uint8 Read(HPort::E port, uint16 pin);
    /// Инициализировать пины данных для приёма из панели
    static void TuneDataPinsToReceive();
};
