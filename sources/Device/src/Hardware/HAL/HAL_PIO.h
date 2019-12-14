#pragma once


#ifdef OLD_VERSION
#define PORT_A2         HPort::_D, HPin::_11
#define PORT_A3         HPort::_D, HPin::_12
#define PORT_A4         HPort::_D, HPin::_13
#define PORT_LF1        HPort::_G, HPin::_2
#define PORT_LF2        HPort::_G, HPin::_3
#define PORT_A1S        HPort::_G, HPin::_4
#define PORT_A0S        HPort::_G, HPin::_5
#define PIN_P2P         HPort::_G, HPin::_1
#define PIN_TESTER_ON   HPort::_F, HPin::_13
#define PIN_TESTER_I    HPort::_G, HPin::_0
#define PIN_TESTER_U    HPort::_F, HPin::_15
#define PIN_TESTER_PNP  HPort::_F, HPin::_14
#else
#define PORT_A2         HPort::_B, HPin::_13
#define PORT_A3         HPort::_G, HPin::_8
#define PORT_A4         HPort::_C, HPin::_6
#define PORT_LF1        HPort::_C, HPin::_7
#define PORT_LF2        HPort::_C, HPin::_8
#define PORT_A1S        HPort::_A, HPin::_8
#define PORT_A0S        HPort::_A, HPin::_10
#define PIN_P2P         HPort::_F, HPin::_11
#define PIN_TESTER_ON   HPort::_A, HPin::_3
#define PIN_TESTER_I    HPort::_B, HPin::_2
#define PIN_TESTER_U    HPort::_F, HPin::_0
#define PIN_TESTER_PNP  HPort::_A, HPin::_6
#endif

#define PORT_A1         HPort::_D, HPin::_10
#define PORT_LFS        HPort::_G, HPin::_6

#define PIN_ADC1_IN2    HPort::_A, HPin::_2
#define PIN_ADC1_IN9    HPort::_B, HPin::_1

#define PIN_ADC3        HPort::_F, HPin::_10
#define PIN_ADC3_IT     HPort::_B, HPin::_11

#define PORT_AT2516_OUT HPort::_C, HPin::_3
#define PORT_AT2516_IN  HPort::_C, HPin::_2
#define PORT_AT2516_CLK HPort::_B, HPin::_10
#define PORT_AT2516_CS  HPort::_B, HPin::_12

#define PORT_AD9286_SCK HPort::_B, HPin::_10
#define PORT_AD9286_DAT HPort::_C, HPin::_3
#define PORT_AD9286_CS  HPort::_E, HPin::_11

#define PORT_CS_RAM     HPort::_G, HPin::_10

#define PIN_DAC1        HPort::_A, HPin::_4

#define PIN_DAC2        HPort::_A, HPin::_5

#define PIN_HCD_DM      HPort::_B, HPin::_14
#define PIN_HCD_DP      HPort::_B, HPin::_15

#define PIN_MODE0       HPort::_A, HPin::_7
#define PIN_MODE1       HPort::_C, HPin::_4
#define PIN_READY       HPort::_G, HPin::_12

#define PIN_PCD_VBUS    HPort::_A, HPin::_9
#define PIN_PCD_DP      HPort::_A, HPin::_12
#define PIN_PCD_DM      HPort::_A, HPin::_11

#define PORT_SPI3_SCK   HPort::_C, HPin::_10
#define PORT_SPI3_DAT   HPort::_C, HPin::_12
#define PORT_SPI3_CS1   HPort::_D, HPin::_3
#define PORT_SPI3_CS2   HPort::_G, HPin::_13

#define PIN_SPI4_CS     HPort::_E, HPin::_11
#define PIN_SPI4_SCK    HPort::_E, HPin::_12
#define PIN_SPI4_MISO   HPort::_E, HPin::_13
#define PIN_SPI4_MOSI   HPort::_E, HPin::_14

#define PIN_TESTER_STR  HPort::_C, HPin::_9
#define PIN_TESTER_DAC  HPort::_A, HPin::_5

#define PIN_USART3_TX   HPort::_D, HPin::_8
#define PIN_USART3_RX   HPort::_D, HPin::_9


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
    /// Установка в "1"
    static void Set(HPort::E port, uint16 pin);
    /// Установка в "0"
    static void Reset(HPort::E port, uint16 pin);
    static void Write(HPort::E port, uint16 pin, HState::E state);
    static bool Read(HPort::E port, uint16 pin);

    static void EXTI_CLEAR_IT_BIT(uint16 pin);
};
