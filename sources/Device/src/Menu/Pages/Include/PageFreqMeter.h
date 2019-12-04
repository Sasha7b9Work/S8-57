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

    bool IsFrequency() const;
    bool IsPeriod() const;
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

    bool Is100ms() const;
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
