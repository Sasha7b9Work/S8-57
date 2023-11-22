// 2023/11/22 13:36:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


struct MathFPGA
{
    static float VoltageCursor(float shiftCurU, Range::E range, int16 rShift);

    static float TimeCursor(float shiftCurT, TBase::E tBase);

    static void CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1, Chan::E ch);
};
