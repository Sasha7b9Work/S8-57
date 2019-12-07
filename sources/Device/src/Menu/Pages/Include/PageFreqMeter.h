#pragma once


class Page;
class Choice;


struct FreqMeterEnabled
{
    enum E
    {
        Off,
        On
    };
    static FreqMeterEnabled::E &Ref();
    operator bool() { return Ref() == On; }
};

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

/// Частота заполняющих импульсов для счёта частоты.
struct FreqMeterFreqClc
{
    enum E
    {
        _100kHz,
        _1MHz,
        _10MHz,
        _100MHz,
        Count
    };

    FreqMeterFreqClc() {};
    operator FreqMeterFreqClc::E();
};

/// Количество периодов.
struct FreqMeterNumberPeriods
{
    enum E
    {
        _1,
        _10,
        _100,
        _1k,
        _10k,
        _100k,
        Count
    };

    FreqMeterNumberPeriods() {};
    operator FreqMeterNumberPeriods::E();
};

struct SettingsFreqMeter
{
    FreqMeterEnabled::E        enabled;
    FreqMeterModeView::E       modeView;
    FreqMeterTimeCounting::E   timeCounting;  ///< Время счёта частоты.
    FreqMeterFreqClc::E        freqClc;       ///< Частота заполнения.
    FreqMeterNumberPeriods::E  numberPeriods; ///< Количество периодов.
};



struct PageFreqMeter
{
    static const Page *const self;

    static void Init();
};
