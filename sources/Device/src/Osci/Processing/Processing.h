#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Processing
{
    /// Аппроксимировать единичное измерение режима рандомизатора функцией sinX/X
    void InterpolationSinX_X(uint8 *data, int numPoints, Osci::Settings::TBase::E tBase);
    /// Возвращает строку автоматического измерения
    String GetStringMeasure(Measure::Type::E measure, Chan::E ch, char *buffer, int lenBuf);
    /// Расчитать все автоматические измерения
    void CalculateMeasures();

    extern int markerTime[Chan::Size][2];

    extern int markerVoltage[Chan::Size][2];
};
