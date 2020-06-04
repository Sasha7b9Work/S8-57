#pragma once
#include "FreqMeter/FreqMeter.h"


class Page;
class Choice;


#define S_FREQ_ENABLED      (set.freq._enabled)


struct SettingsFreqMeter
{
    bool                         _enabled;
    FreqMeter::ModeMeasure::E    _modeMeasure;
    FreqMeter::TimeCounting::E   _timeCounting;  // Время счёта частоты.
    FreqMeter::FreqClc::E        _freqClc;       // Частота заполнения.
    FreqMeter::NumberPeriods::E  _numberPeriods; // Количество периодов.
};



struct PageFreqMeter
{
    static const Page *const self;

    static void Init();
};
