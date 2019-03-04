#pragma once
#include "Display/Display_Types.h"
#include "Settings/Settings.h"
#include <cstring>


/// Специальный класс для сглаживающего фильтра
class Smoother
{
public:
    /// Сгладить данные in и положить их в out
    static void Run(const uint8 *in, uint8 *out, uint numBytes)
    {
        size = (int)numBytes;
        data = in;

        if (ENUM_SMOOTHING.ToNumber() < 2)
        {
            std::memcpy(out, in, numBytes);
        }
        else
        {
            for (uint i = 0; i < numBytes; i++)
            {
                out[i] = CalculatePoint((int)i);
            }
        }
    }
private:
    /// Рассчитывает одну сглаженную точку
    static uint8 CalculatePoint(int index)
    {
        uint sum = 0U;          // Здесь будет храниться сумма

        uint parts = 0U;        // Здесь количество уже просуммированных точек

        index -= ENUM_SMOOTHING.ToNumber() / 2;

        do
        {
            if (index >= 0 && index < size)
            {
                sum += data[index++];
            }
        } while (++parts < ENUM_SMOOTHING.ToNumber());

        return (uint8)(sum / parts);
    }

    static int size;
    const static uint8 *data;
};
