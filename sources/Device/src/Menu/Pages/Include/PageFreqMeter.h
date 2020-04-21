#pragma once
#include "FreqMeter/FreqMeter.h"


class Page;
class Choice;


struct SettingsFreqMeter
{
    uint8                        enabled;
    FreqMeter::ModeView::E       modeView;
    FreqMeter::TimeCounting::E   timeCounting;  // Время счёта частоты.
    FreqMeter::FreqClc::E        freqClc;       // Частота заполнения.
    FreqMeter::NumberPeriods::E  numberPeriods; // Количество периодов.
};



struct PageFreqMeter
{
    static const Page *const self;

    static void Init();
};
