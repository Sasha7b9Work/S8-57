// 2023/11/22 13:35:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "FreqMeter/FreqMeter.h"


namespace DisplayFreqMeter
{
    // Отрисовка значения
    void Update();
};


struct ProgressBarFreqMeter
{
    static void Draw(int x, int y);
};
