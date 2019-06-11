#pragma once


namespace Averager
{
    namespace Tester
    {
        /// Задать количество измерений. Если аргумент отличается от того, который был передан при предыдущем вызове, происходит обнуление.
        void SetCount(int enumAverage);

        void SetDataX(uint16 *data, int step);

        void SetDataY(uint8 *data, int step);
    }
}
