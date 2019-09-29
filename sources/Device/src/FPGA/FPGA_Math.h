#pragma once
#include "Settings/SettingsOsci.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    namespace Math
    {
        float RShift2Abs(int rShift, Range::E range);

        float VoltageCursor(float shiftCurU, Range::E range, uint16 rShift);

        int RShift2Rel(float rShixftAbs, Range::E range);

        float TimeCursor(float shiftCurT, TBase::E tBase);

        void PointsRel2Voltage(const uint8 *points, int numPoints, Range::E range, int16 rShift, float *voltage);

        uint8 Voltage2Point(float voltage, Range::E range, uint16 rShift);

        float Point2Voltage(uint8 value, Range::E range, uint16 rShift);

        void PointsVoltage2Rel(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points);

        void CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);

        float TShift2Abs(int tShift, TBase::E tBase);
    }
};
