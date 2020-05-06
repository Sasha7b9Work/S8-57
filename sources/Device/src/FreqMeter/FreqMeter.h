#pragma once
#include "Utils/Stack.h"


class FreqMeter
{
public:
    
    // Инициализация.
    static void Init();
    
    // Заслать настройки для счётчика частоты
    static void LoadFreqSettings();
    
    // Заслать настрйки для счётчика периода
    static void LoadPeriodSettings();

    static void Update();
    
    // Возвращает измеренное значение частоты
    static float GetFreq();

    static BitSet32 freqActual;             // Здесь хранится последнее действительное значение частоты. Для вывода в режиме частотомера. 0 означает, что значение выводить не надо
    static BitSet32 periodActual;           // Здесь хранится последнее действительное значение периода. Для вывода в режиме частотомера. 0 означает, что значение выводить не надо
    static uint     timeStartMeasureFreq;   // Время начала измерения частоты
    static uint     timeStartMeasurePeriod; // Время начала измерения периода
    static uint     lastFreqRead;           // Последнее время разрешшения чтения частоты
    static uint     lastPeriodRead;         // Последнее время разрешения чтения периода
    static uint     lastFreqOver;           // Последнее время переполнения частоты
    static uint     lastPeriodOver;         // Последнее время переполения периода

    struct FPGA
    {
        static void LoadSettings();
        static void ResetCounterFreq();
        static void ResetCounterPeriod();
        static BitSet32 ReadCounterFreq();
        static BitSet32 ReadCounterPeriod();
    };

private:
    
    // Установить состояние лампочек счётчиков в состояние, соответствующее текущему моменту
    static void SetStateLamps();

    static void SetStateLampFreq();

    static void SetStateLampPeriod();

    static void ReadFreq();

    static void ReadPeriod();

    static float FreqSetToFreq(const BitSet32 *fr);

    static float PeriodSetToFreq(const BitSet32 *period);

public:

    // Частота заполняющих импульсов для счёта частоты.
    struct FreqClc
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

    // Количество периодов.
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
            Count
        };
    };

    // Что показывать - период или частоту
    struct ModeView
    {
#define S_FREQ_MODE_VIEW                (set.freq._modeView)
#define S_FREQ_MODE_VIEW_IS_FREQUENCY   (S_FREQ_MODE_VIEW == FreqMeter::ModeView::Frequency)
#define S_FREQ_MODE_VIEW_IS_PERIOD      (S_FREQ_MODE_VIEW == FreqMeter::ModeView::Period)

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
        enum E
        {
            _100ms,
            _1s,
            _10s,
            Count
        };
    };
};
