// 2023/11/22 13:35:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Stack.h"


namespace FreqMeter
{
    // Инициализация.
    void Init();

    // Заслать настройки для счётчика частоты
    void LoadFreqSettings();

    // Заслать настрйки для счётчика периода
    void LoadPeriodSettings();

    void Update();

    void DrawDebugInfo();
    // Возвращает измеренное значение частоты
    float GetFrequency();

    // При поиске частоты нужно вначале вызывать эту функцию, чтобы очистить измерение. Пока не будет получено измерение, значение, воззвращаемое функцией GetFreq(), будет равно 0.0F
    void ClearMeasure();

    // Возвращает true, если есть измеренное значение частоты
    bool FrequencyIsFound();

    extern BitSet32 freqActual;             // Здесь хранится последнее действительное значение частоты. Для вывода в режиме частотомера. 0 означает, что значение выводить не надо
    extern BitSet32 periodActual;           // Здесь хранится последнее действительное значение периода. Для вывода в режиме частотомера. 0 означает, что значение выводить не надо
    extern uint     timeStartMeasureFreq;   // Время начала измерения частоты
    extern uint     timeStartMeasurePeriod; // Время начала измерения периода
    extern uint     lastFreqRead;           // Последнее время разрешшения чтения частоты
    extern uint     lastPeriodRead;         // Последнее время разрешения чтения периода
    extern uint     lastFreqOver;           // Последнее время переполнения частоты
    extern uint     lastPeriodOver;         // Последнее время переполения периода

    struct FPGA
    {
        static void LoadSettings();
        static void ResetCounterFreq();
        static void ResetCounterPeriod();
        static BitSet32 ReadCounterFreq();
        static BitSet32 ReadCounterPeriod();
    };

    // Частота заполняющих импульсов для счёта частоты.
    struct FreqClc
    {
#define S_FREQ_FREQ_CLC     (set.freq._freqClc)

        enum E
        {
            _100kHz,
            _1MHz,
            _10MHz,
            _100MHz,
            Count
        };
    };

    // Количество периодов.
    struct NumberPeriods
    {
#define S_FREQ_NUMBER_PERIODS   (set.freq._numberPeriods)

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

    // Что показывать - период или частоту
    struct ModeMeasure
    {
#define S_FREQ_MODE_MEASURE                (set.freq._modeMeasure)
#define S_FREQ_MODE_MEASURE_IS_FREQUENCY   (S_FREQ_MODE_MEASURE == FreqMeter::ModeMeasure::Frequency)
#define S_FREQ_MODE_MEASURE_IS_PERIOD      (S_FREQ_MODE_MEASURE == FreqMeter::ModeMeasure::Period)

        enum E
        {
            Frequency,
            Period,
            Count
        };
    };

    // Время счёта периода.
    struct TimeCounting
    {
#define S_FREQ_TIME_COUNTING            (set.freq._timeCounting)
#define S_FREQ_TIME_COUNTING_IS_100ms   (S_FREQ_TIME_COUNTING == FreqMeter::TimeCounting::_100ms)

        enum E
        {
            _100ms,
            _1s,
            _10s,
            Count
        };
    };
};
