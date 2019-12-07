#pragma once
#include "Settings/SettingsOsci.h"


struct Calibrator
{
    static void Calibrate();
    /// Балансировать канал
    static bool Balance(Chan::E ch);

    struct Mode
    {
        enum E
        {
            Freq,
            DC,
            GND
        };
        Mode() {}
        static Mode::E &Ref();
        operator Mode::E() { return Ref(); }
    };

private:

    static bool Calibrate(Chan::E ch);

    static void Balance(Chan::E ch, Range::E range);

    static bool Stretch(Chan::E ch);
};
