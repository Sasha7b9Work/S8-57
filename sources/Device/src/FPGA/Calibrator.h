// 2023/11/22 13:36:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


class Calibrator
{
public:
    // Запускает процедуру калибровки
    static void Calibrate();
    
    // Балансировать канал на всех диапазонах
    static bool BalanceChannel(Chan::E ch, bool showHint);

    struct Mode
    {
#define S_SERV_CALIBRATOR_MODE  (set.serv._calibratorMode)

        enum E
        {
            AC,
            DC,
            GND
        };
    };
};
