#pragma once


namespace HAL
{
    namespace PIO
    {
        struct Port
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

        struct Pin
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

        struct State
        {
            enum E
            {
                Disabled,
                Enabled,
                Size
            };
        };

        struct Mode
        {
            static uint Analog;
            static uint Output_PP;
            static uint RisingIT;
            static uint Input;
        };

        struct Pull
        {
            static uint No;
            static uint Down;
            static uint Up;
        };

        void Init(Port::E port, uint pins, uint mode, uint pull);
        void Set(Port::E port, uint16 pin);
        void Reset(Port::E port, uint16 pin);
        void Write(Port::E port, uint16 pin, State::E state);
        bool Read(Port::E port, uint16 pin);
    }
}
