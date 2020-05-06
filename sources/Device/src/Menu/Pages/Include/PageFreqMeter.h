#pragma once
#include "FreqMeter/FreqMeter.h"


class Page;
class Choice;


#define S_FREQ_ENABLED      (set.freq._enabled)


struct SettingsFreqMeter
{
    bool                         _enabled;
    FreqMeter::ModeView::E       _modeView;
    FreqMeter::TimeCounting::E   timeCounting;  // Время счёта частоты.
    FreqMeter::FreqClc::E        freqClc;       // Частота заполнения.
    FreqMeter::NumberPeriods::E  numberPeriods; // Количество периодов.
};



struct PageFreqMeter
{
    static const Page *const self;

    static void Init();
};
