#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MathFPGA
{
public:

    static float RShift2Abs(int rShift, Range range);
    /// —мещение относительно нулевого в пиксел€х экрана
    static int RShift2Pixels(uint16 rShift, int heightGrid);

    static float VoltageCursor(float shiftCurU, Range range, uint16 rShift);

    static int RShift2Rel(float rShixftAbs, Range range);

    static float TimeCursor(float shiftCurT, TBase tBase);

    static void PointsRel2Voltage(const uint8 *points, int numPoints, Range range, int16 rShift, float *voltage);

    static uint8 Voltage2Point(float voltage, Range range, uint16 rShift);

    static float Point2Voltage(uint8 value, Range range, uint16 rShift);

    static void PointsVoltage2Rel(const float *voltage, int numPoints, Range range, int16 rShift, uint8 *points);

    static void CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);

    static float TShift2Abs(int tShift, TBase tBase);
private:
    static void MultiplyToWindow(float *data, int numPoints);

    static void Normalize(float *data, int numPoints);
    /// ¬озвращает напр€жение, соответствующее верхней границе сетки
    static float MaxVoltageOnScreen(Range range);
};
