#pragma once
#include "Osci/ParametersOsci.h"


struct Calibrator
{
    // Запускает процедуру калибровки
    static void Calibrate();
    
    // Балансировать канал на всех диапазонах
    static bool BalanceChannel(Chan::E ch, bool showHint = true);

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
};
