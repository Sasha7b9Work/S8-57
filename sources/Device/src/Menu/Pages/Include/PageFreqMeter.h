#pragma once
#include "Osci/FreqMeter.h"


class Page;
class Choice;


/// Что показывать - период или частоту
struct FreqMeterModeView
{
    enum E
    {
        Frequency,
        Period,
        Count
    };
    static FreqMeterModeView::E &Ref();
    static bool IsFrequency() { return Ref() == Frequency; }
    static bool IsPeriod()    { return Ref() == Period; }
};

/// Время счёта периода.
struct FreqMeterTimeCounting
{
    enum E
    {
        _100ms,
        _1s,
        _10s,
        Count
    };

    FreqMeterTimeCounting() {};
    static FreqMeterTimeCounting::E &Ref();
    operator FreqMeterTimeCounting::E() { return Ref(); }
    static bool Is100ms()               { return Ref() == _100ms; }
};


struct SettingsFreqMeter
{
    FreqMeter::Enabled::E        enabled;
    FreqMeterModeView::E         modeView;
    FreqMeterTimeCounting::E     timeCounting;  ///< Время счёта частоты.
    FreqMeter::FreqClc::E        freqClc;       ///< Частота заполнения.
    FreqMeter::NumberPeriods::E  numberPeriods; ///< Количество периодов.
};



struct PageFreqMeter
{
    static const Page *const self;

    static void Init();
};
