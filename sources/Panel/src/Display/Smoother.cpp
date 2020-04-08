#include "defines.h"
#include "common/Command.h"
#include "Display/Smoother.h"


static uint16 outX[TESTER_NUM_POINTS];
static uint8 outY[TESTER_NUM_POINTS];


// Получить сглаженно значение точки в позиции pos массива data со сглаживанием по smooth точкам
template<class T>
static void Smooth(int pos, const T *in, T *out, int smooth);


void SmootherTester::Run(const uint16 *x, const uint8 *y, int smooth)
{
    smooth++;

    for(int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        Smooth<uint16>(i, x, outX, smooth);
        Smooth<uint8>(i, y, outY, smooth);
    }
}


template<class T>
static void Smooth(int pos, const T *in, T *out, int smooth)
{
    if(smooth == 1)
    {
        out[pos] = in[pos];
        return;
    }

    if((pos < smooth / 2) || (pos < 10))
    {
        out[pos] = in[pos];
    }
    else if((pos > TESTER_NUM_POINTS - smooth / 2))
    {
        out[pos] = in[pos];
    }
    else
    {
        float value = 0.0F;

        int count = 0;

        for(int i = pos - smooth / 2; i < pos + smooth / 2; i++)
        {
            if(i < TESTER_NUM_POINTS)
            {
                count++;
                value += in[i];
            }
        }

        out[pos] = value / count;
    }
}


uint16 *SmootherTester::X()
{
    return outX;
}


uint8 *SmootherTester::Y()
{
    return outY;
}
