#pragma once
#include "FPGA/FPGA_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    namespace Math
    {
        float RShift2Abs(int rShift, Osci::Settings::Range::E range);

        float VoltageCursor(float shiftCurU, Osci::Settings::Range::E range, uint16 rShift);

        int RShift2Rel(float rShixftAbs, Osci::Settings::Range::E range);

        float TimeCursor(float shiftCurT, Osci::Settings::TBase::E tBase);

        void PointsRel2Voltage(const uint8 *points, int numPoints, Osci::Settings::Range::E range, int16 rShift, float *voltage);

        uint8 Voltage2Point(float voltage, Osci::Settings::Range::E range, uint16 rShift);

        float Point2Voltage(uint8 value, Osci::Settings::Range::E range, uint16 rShift);

        void PointsVoltage2Rel(const float *voltage, int numPoints, Osci::Settings::Range::E range, int16 rShift, uint8 *points);

        void CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);

        float TShift2Abs(int tShift, Osci::Settings::TBase::E tBase);
    }
};
