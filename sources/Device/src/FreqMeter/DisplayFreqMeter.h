#pragma once
#include "FreqMeter/FreqMeter.h"


struct DisplayFreqMeter
{
    // Отрисовка значения
    static void Update();

private:
    // Выводит отладочную информацию
    static void DrawDebugInfo();
};


struct ProgressBarFreqMeter
{
    static void Draw(int x, int y);
};
