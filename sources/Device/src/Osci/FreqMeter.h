#pragma once
//#include "Menu/Pages/Include/PageFreqMeter.h"
#include "Utils/Stack.h"


class FreqMeter
{
    friend struct DisplayFreqMeter;
    friend struct ProgressBarFreqMeter;
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
    /// Здесь хранится последнее действительное значение частоты. Для вывода в режиме частотомера. 0 означает, что значение выводить не надо
    static BitSet32 freqActual;
    /// Здесь хранится последнее действительное значение периода. Для вывода в режиме частотомера. 0 означает, что значение выводить не надо
    static BitSet32 periodActual;
    /// Последнее время разрешшения чтения частоты
    static uint lastFreqRead;
    /// Последнее время разрешения чтения периода
    static uint lastPeriodRead;
    /// Последнее время переполнения частоты
    static uint lastFreqOver;
    /// Последнее время переполения периода
    static uint lastPeriodOver;
    /// Время начала измерения частоты
    static uint timeStartMeasureFreq;
    /// Время начала измерения периода
    static uint timeStartMeasurePeriod;

public:
    struct Enabled
    {
        enum E
        {
            Off,
            On
        };
        static Enabled::E &Ref();
        operator bool() { return Ref() == On; }
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
            Count
        };

        FreqClc() {};
        operator FreqClc::E();
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
            Count
        };

        NumberPeriods() {};
        operator NumberPeriods::E();
    };

    /// Что показывать - период или частоту
    struct ModeView
    {
        enum E
        {
            Frequency,
            Period,
            Count
        };
        static ModeView::E &Ref();
        static bool IsFrequency() { return Ref() == Frequency; }
        static bool IsPeriod() { return Ref() == Period; }
    };

    /// Время счёта периода.
    struct TimeCounting
    {
        enum E
        {
            _100ms,
            _1s,
            _10s,
            Count
        };

        TimeCounting() {};
        static TimeCounting::E &Ref();
        operator TimeCounting::E() { return Ref(); }
        static bool Is100ms() { return Ref() == _100ms; }
    };
};


struct DisplayFreqMeter
{
    /// Отрисовка значения
    static void Draw();

private:

    static void DrawFrequency(int x, int _y);

    static void DrawPeriod(int x, int y);
    /// Выводит отладочную информацию
    static void DrawDebugInfo();

    static pString FreqSetToString(const BitSet32 *fr);

    static pString PeriodSetToString(const BitSet32 *pr);
    /// Возвращает порядок младшего разряда считанного значения счётчика периода при данных настройках
    static int LowOrder(FreqMeter::FreqClc::E freqCLC, FreqMeter::NumberPeriods::E numPeriods);
    /// Преобразует 6 разрядов числа, хранящиеся в стеке, в текстовую строку периода. Младший значащий разряд хранится на вершине стека. order - его порядок
    static pString StackToString(Stack<uint> *stack, int order);
    /// Записывает 6 разрядов из стека stack в буфер buffer. Младший разряд на вершине стека. Точку ставить на point позиции, начиная с buffer[0]
    static void WriteStackToBuffer(Stack<uint> *stack, int point, const char *suffix);
};


struct ProgressBarFreqMeter
{
    static void Draw(int x, int y);
};
