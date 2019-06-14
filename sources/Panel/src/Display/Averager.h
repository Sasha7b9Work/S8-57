#pragma once


namespace Averager
{
    namespace Tester
    {
        /// Задать количество измерений. Если аргумент отличается от того, который был передан при предыдущем вызове, происходит обнуление.
        void SetCount(int enumAverage);

        void Process(const uint16 *x, const uint8 *y, int step);

        uint16 *X();
        uint8 *Y();
    }
}
