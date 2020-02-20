#pragma once
#include "Settings/ParametersOsci.h"


struct Calibrator
{
    /// Запускает процедуру калибровки
    static void Calibrate();
    /// Балансировать канал на всех диапазонах
    static bool Balance(Chan::E ch, bool showHint = true);

private:
    /// Откалибровать канал Ch
    static bool Calibrate(Chan::E ch);
    /// Балансировать канал на одном диапазоне
    static void Balance(Chan::E ch, Range::E range);
    /// "Растянуть" канал
    static bool Stretch(Chan::E ch);
    /// Найти коэффициент растяжки канала
    static float FindStretchK(Chan::E ch);

    static void NormalExit();

    static void BadExit();

public:

    struct Mode
    {
        enum E
        {
            Freq,
            DC,
            GND
        };
        Mode() {}
        static Mode::E &Ref();
        operator Mode::E() { return Ref(); }
    };
};
