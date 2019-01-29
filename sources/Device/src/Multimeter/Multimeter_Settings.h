#pragma once


namespace Multimeter
{
    namespace Settings
    {
        /// АВП - автовыбор предела
        struct AVP
        {
            enum E
            {
                Off,
                On
            } value;
            explicit AVP(E v) : value(v) { };
        };

        /// Предел имзерения постоянного напряжения
        struct RangeDC
        {
            enum E
            {
                _2V,
                _20V,
                _500V
            } value;
            explicit RangeDC(E v) : value(v) { };
        };

        /// Предел измерения переменного напряжения
        struct RangeAC
        {
            enum E
            {
                _2V,
                _20V,
                _400V
            } value;
            explicit RangeAC(E v) : value(v) {};
        };

        /// Предел измерения сопротивленя постоянному току
        struct RangeResistance
        {
            enum E
            {
                _2k,
                _20k,
                _200k,
                _10M
            } value;
            explicit RangeResistance(E v) : value(v) {};
        };
    }
}
