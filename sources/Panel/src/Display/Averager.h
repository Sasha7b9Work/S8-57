#pragma once


struct AveragerTester
{
    /// Задать количество измерений. Если аргумент отличается от того, который был передан при предыдущем вызове, происходит обнуление.
    static void SetCount(int enumAverage);

    static void Process(const uint16 *x, const uint8 *y, int step);

    static uint16 *X();
    static uint8 *Y();
};
