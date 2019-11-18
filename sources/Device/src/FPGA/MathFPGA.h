#pragma once
#include "Settings/SettingsOsci.h"


struct MathFPGA
{
    static float RShift2Abs(int16 rShift, Range::E range);

    static float VoltageCursor(float shiftCurU, Range::E range, int16 rShift);

    static int16 RShift2Rel(float rShixftAbs, Range::E range);

    static float TimeCursor(float shiftCurT, TBase::E tBase);

    static void PointsRel2Voltage(const uint8 *points, uint numPoints, Range::E range, int16 rShift, float *voltage);

    static uint8 Voltage2Point(float voltage, Range::E range, int16 rShift);

    static float Point2Voltage(uint8 value, Range::E range, int16 rShift);

    static void PointsVoltage2Rel(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points);

    static void CalculateFFT(float *data, uint numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);

    static float TShift2Abs(int tShift, TBase::E tBase);
};
