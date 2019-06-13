#pragma once


namespace Averager
{
    namespace Tester
    {
        /// Задать количество измерений. Если аргумент отличается от того, который был передан при предыдущем вызове, происходит обнуление.
        void SetCount(int enumAverage);

        void ProcessX(const uint16 *data, int step);

        void ProcessY(const uint8 *data, int step);

        uint16 *X();
        uint8 *Y();
    }
}
