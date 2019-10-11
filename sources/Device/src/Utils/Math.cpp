#include "defines.h"
#include "log.h"
#include "Math.h"
#include "Settings/Settings.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>

#include "Utils/Values.h"
#include "FPGA/FPGA.h"



template void  Math::Swap<int>(int *, int *);
template int   Math::Sign<int>(int);
template int   Math::Sign<int8>(int8);
template int   Math::Sign<int16>(int16);
template void  Math::CircleIncrease<int8>(int8 *, int8, int8);
template void  Math::CircleIncrease<uint8>(uint8 *, uint8, uint8);
template void  Math::CircleIncrease<int16>(int16 *, int16, int16);
template void  Math::CircleIncrease<int>(int *, int, int);
template void  Math::CircleDecrease<int8>(int8 *, int8, int8);
template void  Math::CircleDecrease<uint8>(uint8 *, uint8, uint8);
template void  Math::CircleDecrease<int>(int *, int, int);
template void  Math::CircleDecrease<int16>(int16 *, int16, int16);
template int16 Math::Abs<int16>(int16);
template int   Math::Abs<int>(int);
template void  Math::LimitationIncrease<uint8>(uint8 *, uint8);
template void  Math::LimitationDecrease<uint8>(uint8 *, uint8);
template void  Math::Limitation<float>(float *, float, float);
template void  Math::Limitation<uint16>(uint16 *, uint16, uint16);
template void  Math::Limitation<int>(int *, int, int);
template void  Math::Limitation<uint8>(uint8 *, uint8, uint8);
template float Math::LimitationRet<float>(float, float, float);
template uint8 Math::LimitationRet<uint8>(uint8, uint8, uint8);
template void  Math::AdditionThisLimitation<uint16>(uint16 *, int, uint16, uint16);
template float Math::Max<float>(float, float, float);
template int   Math::Min<int>(int, int);
template uint  Math::Min<uint>(uint, uint);



int Math::LowSignedBit(uint value)
{
    int verValue = 1;

    for (int i = 0; i < 32; i++)
    {
        if (verValue & (static_cast<int>(value)))
        {
            return i;
        }

        verValue <<= 1;
    }

    return -1;
}


void Math::Smoothing(uint8 *data, int numPoints, int numSmooth)
{
    if (numSmooth == 0 || numSmooth == 1)
    {
        return;
    }

    float *buffer = (float *)std::malloc((uint)(numPoints * static_cast<int>(sizeof(float))));
    int  *num = (int *)std::malloc((uint)(numPoints * static_cast<int>(sizeof(int))));

    if (num != 0 && buffer != 0)
    {
        for (int i = 1; i < numPoints; i++)
        {
            buffer[i] = 0.0F;

            num[i] = 0;
            for (int j = -numSmooth / 2; j < numSmooth / 2; j++)
            {
                int index = i + j;
                if (index >= 1 && index < numPoints)
                {
                    buffer[i] += data[index];
                    ++num[i];
                }
            }
        }

        for (int i = 1; i < numPoints; i++)
        {
            data[i] = (uint8)(buffer[i] / num[i] + 0.5F);
        }
    }

    std::free(buffer);
    std::free(num);
}


template<class T>
T Math::Max(T val1, T val2, T val3)
{
    T retValue = val1;
    if (val2 > retValue)
    {
        retValue = val2;
    }
    if (val3 > retValue)
    {
        retValue = val3;
    }
    return retValue;
}


template<class T>
T Math::Min(T x1, T x2)
{
    return (x1 < x2) ? x1 : x2;
}


int Math::MinFrom2Int(int val0, int val1)
{
    if (val0 < val1)
    {
        return val0;
    }
    return val1;
}


int Math::Pow10(int pow)
{
    int retValue = 1;

    while (pow--)
    {
        retValue *= 10;
    }

    return retValue;
}


uint8 Math::MaxFromArray(const uint8 *data, int firstPoint, int lastPoint)
{

#define MAX_IF_ABOVE if(d > max) { max = d; }

    uint8 max = 0;
    const uint8 *pointer = &data[firstPoint];

    for (int i = firstPoint; i < lastPoint; i += 2)
    {
        uint8 d = *pointer++;
        MAX_IF_ABOVE;
        d = *pointer++;
        MAX_IF_ABOVE;
    }
    if ((lastPoint - firstPoint + 1) & 1)
    {
        uint8 d = *pointer;
        MAX_IF_ABOVE
    }

    return max;
}


uint8 Math::MinFromArray(const uint8 *data, int firstPoint, int lastPoint)
{

#define MIN_IF_LESS if(d < min) { min = d; }

    uint8 min = 255;
    const uint8 *pointer = &data[firstPoint];

    for (int i = firstPoint; i < lastPoint; i += 2)
    {
        uint8 d = *pointer++;
        MIN_IF_LESS
        d = *pointer++;
        MIN_IF_LESS
    }
    if ((lastPoint - firstPoint + 1) & 1)
    {
        uint8 d = *pointer;
        MIN_IF_LESS
    }

    return min;
}


void Math::MinMaxFromArray(const uint8 *data, int firstPoint, int lastPoint, uint8 *outMin, uint8 *outMax)
{
    uint8 min = 255;
    uint8 max = 0;

    const uint8 *pointer = &data[firstPoint];

    for (int i = firstPoint; i < lastPoint; i += 2)
    {
        uint8 d = *pointer++;
        MIN_IF_LESS
        MAX_IF_ABOVE
        d = *pointer++;
        MIN_IF_LESS
        MAX_IF_ABOVE
    }
    if ((lastPoint - firstPoint + 1) & 1)
    {
        uint8 d = *pointer;
        MIN_IF_LESS
        MAX_IF_ABOVE
    }

    *outMin = min;
    *outMax = max;
}


uint8 Math::MaxFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint)
{
    uint8 max = Math::MaxFromArray(data, firstPoint, lastPoint);
    if (max >= VALUE::MAX)
    {
        max = Uint8::ERROR;
    }
    return max;
}


uint8 Math::MinFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint)
{
    uint8 min = Math::MinFromArray(data, firstPoint, lastPoint);
    if (min < VALUE::MIN || min >= VALUE::MAX)
    {
        min = Uint8::ERROR;
    }
    return min;
}


float Math::GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine)
{
    if (y0 == y1)
    {
        return (float)x1;
    }

    return (yHorLine - y0) / ((float)(y1 - y0) / (float)(x1 - x0)) + x0;
}


void Math::CalculateMathFunction(float *dataAandResult, const float *dataB, int numPoints)
{
    if (set.math.function == MathFunction::Sum)
    {
        int delta = dataB - dataAandResult;
        float *end = &dataAandResult[numPoints];
        while (dataAandResult < end)
        {
            *dataAandResult += *(dataAandResult + delta);
            dataAandResult++;
        }
    }
    else if (set.math.function == MathFunction::Mul) //-V547
    {
        int delta = dataB - dataAandResult;
        float *end = &dataAandResult[numPoints];
        while (dataAandResult < end)
        {
            *dataAandResult *= *(dataAandResult + delta);
            dataAandResult++;
        }
    }
    else
    {
        // здесь ничего
    }
}


uint8 Math::CalculateFiltr(const uint8 *data, int x, int numPoints, int numSmoothing)
{
    if (numSmoothing < 2)
    {
        return data[x];
    }

    int count = 1;
    int sum = data[x];
    int startDelta = 1;

    int endDelta = numSmoothing / 2;

    for (int delta = startDelta; delta <= endDelta; delta++)
    {
        if (((x - delta) >= 0) && ((x + delta) < (numPoints)))
        {
            sum += data[x - delta];
            sum += data[x + delta];
            count += 2;
        }
    }

    if ((numSmoothing % 2) == 1)
    {
        int delta = numSmoothing / 2;
        if ((x + delta) < numPoints)
        {
            sum += data[x + delta];
            count++;
        }
    }

    return (uint8)(sum / (float)count);
}


void Math::CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, int numSmoothing)
{
    if (numSmoothing < 2)
    {
        std::memcpy(dataOut, dataIn, (uint)numPoints); //-V107
    }
    else
    {
        bool addCalculation = (numSmoothing % 2) == 1;
        int endDelta = numSmoothing / 2;
        int startDelta = 1;
        int d = numSmoothing / 2;

        for (int i = 0; i < numPoints; i++)
        {
            int count = 1;
            int sum = dataIn[i];

            for (int delta = startDelta; delta <= endDelta; delta++)
            {
                if (((i - delta) >= 0) && ((i + delta) < (numPoints)))
                {
                    sum += dataIn[i - delta];
                    sum += dataIn[i + delta];
                    count += 2;
                }
            }

            if (addCalculation)
            {
                if ((i + d) < numPoints)
                {
                    sum += dataIn[i + d];
                    count++;
                }
            }

            dataOut[i] = (uint8)(sum / (float)count);
        }
    }
}


int Math::FindAnotherElement(const uint8 *data, uint8 value, int numElements)
{
    for (int i = 0; i < numElements; i++)
    {
        if (data[i] != value)
        {
            return i;
        }
    }

    return -1;
}


int Math::DigitsInIntPart(float value)
{
    if (value == std::numeric_limits<float>::infinity())
    {
        LOG_WRITE("Nan value %f", value);
        return 2;
    }

    float absValue = std::fabsf(value);

    int num = 0;

    do
    {
        ++num;
        absValue /= 10.0F;
    } while (absValue >= 1.0F);

    return num;
}


float Math::RoundFloat(float value, int numDigits)
{
    if(value == std::numeric_limits<float>::infinity())
    {
        value = value;
    }
    
    float absValue = std::fabsf(value);

    int digsInInt = Math::DigitsInIntPart(absValue);

    if (digsInInt < numDigits)  // Подстрахуемся
    {
        int pow = Pow10(numDigits - digsInInt);
        absValue = (static_cast<int>(absValue * pow + 0.5F)) / (float)pow;
    }

    return value > 0.0F ? absValue : -absValue;
}


bool Math::IsEquals(float x, float y)
{
    return std::fabsf(x - y) < std::numeric_limits<float>::epsilon();
}


bool Math::FloatsIsEquals(float value0, float value1, float epsilonPart)
{
    float max = std::fabsf(value0) > std::fabsf(value1) ? std::fabsf(value0) : std::fabsf(value1);

    float epsilonAbs = max * epsilonPart;

    return std::fabsf(value0 - value1) < epsilonAbs;
}


template<class T>
void Math::Swap(T *value0, T *value1)
{
     T temp = *value0; *value0 = *value1; *value1 = temp;
}


template<class T>
void Math::Sort(T *value0, T *value1)
{
     if (*value1 < *value0)
     {
         Swap<T>(value0, value1);
     } 
}


template<class T>
int Math::Sign(T x)
{
     if (x > (T)(0))
     {
         return 1; 
     }
     if (x < ((T)(0)))
     {
         return -1; 
     }
     return 0; 
}


template<class T>
T Math::Abs(T x)
{
     if (x < (T)0)
     {
         return -x; 
     }
     return x; 
}


template<class T>
void Math::CircleIncrease(T *value, T min, T max)
{
    if (*value < max)
    {
        ++(*value);
    }
    else
    {
        *value = (T)min;
    }
}


template<class T>
void Math::CircleDecrease(T *value, T min, T max)
{
    if (*value > min)
    {
        --(*value);
    }
    else
    {
        *value = (T)max;
    }
}


template<class T>
void Math::LimitationIncrease(T *value, T max)
{
    if ((*value) < max)
    {
        ++(*value);
    }
}


template<class T>
void Math::Limitation(T *value, T min, T max)
{
    if (*value < min)
    {
        *value = min;
    }
    else if (*value > max)
    {
        *value = max;
    }
    else
    {
        // значения совпадают - ничего не делаем
    }
}


template<class T>
void Math::LimitationDecrease(T *value, T min)
{
    if (*value > min)
    {
        --(*value);
    }
}


template<class T>
T Math::LimitationRet(T value, T min, T max)
{
    if (value < min)
    {
        return min;
    }
    if (value > max)
    {
        return max;
    }
    return value;
}


template<class T>
void Math::AdditionThisLimitation(T *value, int term, T min, T max)
{
    if (term > 0)
    {
        if (max - term >= *value)
        {
            *value += (T)term;
        }
        else
        {
            *value = max;
        }
    }
    else
    {
        if (min - term <= *value)
        {
            *value += (T)term;
        }
        else
        {
            *value = min;
        }
    }
}


float Math::RandFloat(float min, float max)
{
    float range = max - min;
    float scale = range / RAND_MAX;
    return min + scale * std::rand();
}
