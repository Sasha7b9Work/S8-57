// 2023/11/22 13:36:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


namespace MathFPGA
{
    float VoltageCursor(float shiftCurU, Range::E range, int16 rShift);

    float TimeCursor(float shiftCurT, TBase::E tBase);

    void CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1, Chan::E ch);
};
