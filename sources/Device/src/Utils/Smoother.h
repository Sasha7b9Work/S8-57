#pragma once
#include "Settings/Settings.h"
#include <cstring>


// Специальный класс для сглаживающего фильтра
namespace Smoother
{
    static int size = 0;
    static const uint8 *data = nullptr;
    static int numSmooth = 0;

    // Рассчитывает одну сглаженную точку
    inline uint8 CalculatePoint(int index)
    {
        uint sum = 0U;          // Здесь будет храниться сумма

        int parts = 0;        // Здесь количество уже просуммированных точек

        index -= numSmooth / 2;

        do
        {
            if (index >= 0 && index < size)
            {
                sum += data[index++];
            }
        } while (++parts < numSmooth);

        return static_cast<uint8>(static_cast<float>(sum) / parts + 0.5F);
    }

    // Сгладить данные in и положить их в out
    inline void Run(const uint8 *in, uint8 *out, uint numBytes, int _numSmooth)
    {
        numSmooth = _numSmooth;
        size = static_cast<int>(numBytes);
        data = in;

        if (numSmooth < 2)
        {
            std::memcpy(out, in, numBytes);
        }
        else
        {
            for (uint i = 0; i < numBytes; i++)
            {
                out[i] = CalculatePoint(static_cast<int>(i));
            }
        }
    }
};
