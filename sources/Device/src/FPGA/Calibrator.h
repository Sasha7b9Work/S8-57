#pragma once
#include "Osci/ParametersOsci.h"


class Calibrator
{
public:
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
    };
};
