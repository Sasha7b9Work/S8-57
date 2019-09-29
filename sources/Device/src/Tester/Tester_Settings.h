#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Tester
{
    namespace Settings
    {
        struct Smoothing
        {
            enum E
            {
                _1,
                _2,
                _3,
                _4,
                _5
            } value;
            explicit Smoothing(E v) : value(v) {};
        };

        /// Масштаб
        struct Scale
        {
            enum E
            {
                _2uA,
                _5uA,
                _10uA,
                _20uA,
                _50uA,
                _100uA,
                _200uA,
                _500uA,
                _1mA,
                _2mA,
                _5mA,
                _10mA,
                _20mA,
                Number
            } value;
            Chan::E ch;
            explicit Scale(Osci::Settings::Range::E range, Chan::E _ch) : value(static_cast<E>(range)), ch(_ch) {};
            pString ToString() const;
        };

        /// Смещение
        struct Shift
        {
            Shift(uint16 rShift, Chan::E _ch) : shift(rShift), ch(_ch) {};
            String ToString(Scale::E scale);
        private:
            uint16 shift;
            Chan::E ch;
        };
    }
}
