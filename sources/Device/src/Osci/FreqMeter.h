#pragma once
#include "Menu/Pages/Include/PageFreqMeter.h"
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
    static int LowOrder(FreqMeterFreqClc::E freqCLC, FreqMeterNumberPeriods::E numPeriods);
    /// Преобразует 6 разрядов числа, хранящиеся в стеке, в текстовую строку периода. Младший значащий разряд хранится на вершине стека. order - его порядок
    static pString StackToString(Stack<uint> *stack, int order);
    /// Записывает 6 разрядов из стека stack в буфер buffer. Младший разряд на вершине стека. Точку ставить на point позиции, начиная с buffer[0]
    static void WriteStackToBuffer(Stack<uint> *stack, int point, const char *suffix);
};


struct ProgressBarFreqMeter
{
    static void Draw(int x, int y);
};
