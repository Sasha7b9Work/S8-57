#pragma once
#include "Utils/Stack.h"


#define FREQ_METER_ENABLED                  (set.freq_enabled)
#define FREQ_METER_IS_ENABLED               (FREQ_METER_ENABLED.value == FrequencyCounter::Enabled::On)
#define FREQ_METER_MODE_VIEW                (set.freq_modeView)
#define FREQ_METER_MODE_VIEW_IS_FREQUENCY   (FREQ_METER_MODE_VIEW.value == FrequencyCounter::ModeView::Frequency)
#define FREQ_METER_MODE_VIEW_IS_PERIOD      (FREQ_METER_MODE_VIEW.value == FrequencyCounter::ModeView::Period)
#define FREQ_METER_TIMECOUNTING             (set.freq_timeCounting)
#define FREQ_METER_FREQ_CLC                 (set.freq_freqClc)
#define FREQ_METER_NUM_PERIODS              (set.freq_numberPeriods)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FrequencyCounter
{
public:
    /// Инициализация.
    static void Init();
    /// Заслать настройки для счётчика частоты
    static void LoadFreqSettings();
    /// Заслать настрйки для счётчика периода
    static void LoadPeriodSettings();

    static void Update();
    /// Возвращает измеренное значение частоты
    static float GetFreq();
    /// Отрисовка значения
    static void Draw();

    /// Время счёта периода.
    struct TimeCounting
    {
        enum E
        {
            _100ms,
            _1s,
            _10s
        } value;
        operator uint8() const { return (uint8)value; }
    };

    /// Частота заполняющих импульсов для счёта частоты.
    struct FreqClc
    {
        enum E
        {
            _100kHz,
            _1MHz,
            _10MHz,
            _100MHz,
            Number
        } value;
        operator uint8() const { return (uint8)value; }
    };

    /// Количество периодов.
    struct NumberPeriods
    {
        enum E
        {
            _1,
            _10,
            _100,
            _1k,
            _10k,
            _100k,
            Number
        } value;
        operator uint8() const { return (uint8)value; }
    };

    struct Enabled
    {
        enum E
        {
            Off,
            On
        } value;
        uint8 ToUINT8() const { return (uint8)value; };
    };

    /// Что показывать - период или частоту
    struct ModeView
    {
        enum E
        {
            Frequency,
            Period,
            Number
        } value;
    };

private:
    /// Установить состояние лампочек счётчиков в состояние, соответствующее текущему моменту
    static void SetStateLamps();

    static void SetStateLampFreq();

    static void SetStateLampPeriod();

    static void ReadFreq();

    static void ReadPeriod();

    static void LoadSettings();

    static float FreqSetToFreq(const BitSet32 *fr);

    static float PeriodSetToFreq(const BitSet32 *period);
    /// Установленное в true значение означает, что частоту нужно считать по счётчику периода
    static bool readPeriod;

    static float prevFreq;

    static float frequency;

    static float period;
};
