#pragma once
#include "Settings/SettingsOsci.h"


struct Calibrator
{
    static void Calibrate();
    /// Балансировать канал
    static bool Balance(Chan::E ch);

private:

    static bool Calibrate(Chan::E ch);

    static void BalanceChannel(Chan::E ch, Range::E range);

    static bool Stretch(Chan::E ch);
};
