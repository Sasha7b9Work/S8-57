#pragma once
#include "Settings/SettingsOsci.h"


struct Calibrator
{
    static void Calibrate();
    /// Балансировать канал
    static void Balance(Chan::E ch);

private:

    static void BalanceChannel(Chan::E ch, Range::E range);
};
