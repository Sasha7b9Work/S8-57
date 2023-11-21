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
