#pragma once
#include "FreqMeter/FreqMeter.h"


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
