// 2023/11/22 13:44:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "FreqMeter/FreqMeter.h"


class Page;
class Choice;


#define S_FREQ_METER_ENABLED    (set.freq._enabled)


struct SettingsFreqMeter
{
    bool                         _enabled;          // Включён ли режим частотомера
    FreqMeter::ModeMeasure::E    _modeMeasure;      // Режим измерения - период или частота
    FreqMeter::TimeCounting::E   _timeCounting;     // Время счёта частоты.
    FreqMeter::FreqClc::E        _freqClc;          // Частота заполнения.
    FreqMeter::NumberPeriods::E  _numberPeriods;    // Количество периодов.
};



struct PageFreqMeter
{
    static const Page *const self;

    static void Init();
};
