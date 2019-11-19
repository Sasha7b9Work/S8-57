#pragma once
#include "Settings/SettingsOsci.h"


struct MathFPGA
{
    static float VoltageCursor(float shiftCurU, Range::E range, int16 rShift);

    static float TimeCursor(float shiftCurT, TBase::E tBase);

    static uint8 Voltage2Point(float voltage, Range::E range, int16 rShift);

    static float Point2Voltage(uint8 value, Range::E range, int16 rShift);

    static void PointsVoltage2Rel(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points);

    static void CalculateFFT(float *data, uint numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);
};
