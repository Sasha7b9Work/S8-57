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
};

/// Время счёта периода.
struct FreqMeterTimeCounting
{
    enum E
    {
        _100ms,
        _1s,
        _10s
    };
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

    /// \todo Здесь страшенный говнокод. Нельзя опускаться до персональных проверок

    /// Возвращает указатель на ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Время счёта F
    static const Choice *GetChoiceTimeF();
    /// Возвращает указаетль на ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Метки времени
    static const Choice *GetChoiceFreqClc();
    /// Возвращает указатель на ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Кол-во периодов
    static const Choice *GetChoiceNumPeriods();
};
