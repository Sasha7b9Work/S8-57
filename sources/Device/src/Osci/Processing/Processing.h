#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Processing
{
    friend class Measure;

public:
    /// Аппроксимировать единичное измерение режима рандомизатора функцией sinX/X
    static void InterpolationSinX_X(uint8 *data, int numPoints, Osci::Settings::TBase::E tBase);
    /// Возвращает строку автоматического измерения
    static String GetStringMeasure(Measure::Type::E measure, Chan::E ch, char *buffer, int lenBuf);
    /// Расчитать все автоматические измерения
    static void CalculateMeasures();

    static int markerTime[Chan::Size][2];

    static int markerVoltage[Chan::Size][2];

private:
    /// Приведение сигнала к установленным в приборе настройкам
    static void CountedToCurrentSettings();
    /// Приведение сигнала к текущим Range и rShift
    static void CountedRange(Chan::E ch);
    /// Приведение сигнала к текущему TBase
    static void CountedTBase();
    /// Приведение сигнала к текущему tShift
    static void CountedTShift();
    /// Приведение количества точек в сигнале к установленным настройкам
    static void CountedEnumPoints();
};
