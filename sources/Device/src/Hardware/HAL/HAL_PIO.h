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
        Size
    } value;
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

namespace HAL
{
    namespace PIO
    {
        struct Pull
        {
            enum E
            {
                No,
                Down,
                Up,
                Size
            };
        };

        struct Speed
        {
            enum E
            {
                Low,
                VeryHigh,
                Size
            };
        };

        struct Alternate
        {
            enum E
            {
                AF0_MCO,
                AF7_USART3,
                Speed
            };
        };

        void Init(HPort::E port, uint pins, HMode::E mode, Pull::E pull, Speed::E speed = Speed::Low, Alternate::E alternate = Alternate::AF0_MCO);
        /// Установка в "1"
        void Set(HPort::E port, uint16 pin);
        /// Установка в "0"
        void Reset(HPort::E port, uint16 pin);
        void Write(HPort::E port, uint16 pin, HState::E state);
        bool Read(HPort::E port, uint16 pin);

        void EXTI_CLEAR_IT_BIT(uint16 pin);
    }
}
