#include "defines.h"
#include "Log.h"
#include "Command.h"
#include "Averager.h"
#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int enumAve = 0;

struct StructX
{
    /// Усреднённые данные одного шага
    uint data[TESTER_NUM_POINTS];
    /// Количество измерений, находящихся в data
    int cout;
};

struct StructY
{
    /// Усреднённые данные одного шага
    uint16 data[TESTER_NUM_POINTS];
    /// Количество измерений, находящихся в data
    int cout;
};

/// Это отрисовываемые данные
static uint16 x[TESTER_NUM_POINTS] __attribute__((section("CCM_DATA")));
static uint8 y[TESTER_NUM_POINTS] __attribute__((section("CCM_DATA")));

static StructX dataX[5] __attribute__((section("CCM_DATA")));
static StructY dataY[5] __attribute__((section("CCM_DATA")));

static StructX oldX[5] __attribute__((section("CCM_DATA")));
static StructY oldY[5] __attribute__((section("CCM_DATA")));

static StructX currentX[5] __attribute__((section("CCM_DATA")));
static StructY currentY[5] __attribute__((section("CCM_DATA")));


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Copy16to32(const uint16 *data, uint *buffer)
{
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        buffer[i] = data[i];
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Copy8to16(const uint8 *data, uint16 *buffer)
{
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        buffer[i] = data[i];
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Averager::Tester::SetCount(int count)
{
    if (enumAve != count)
    {
        for (int step = 0; step < 5; step++)
        {
            for (int i = 0; i < TESTER_NUM_POINTS; i++)
            {
                dataX[step].data[i] = 0;
                dataY[step].data[i] = 0;
            }
        }
    }

    enumAve = count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int Difference(int step)
{
    int max = 0;

    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        int diffX = (int)oldX[step].data[i] - (int)currentX[step].data[i];
        if (diffX < 0)
        {
            diffX = -diffX;
        }
        if (diffX > max)
        {
            max = diffX;
        }

        int diffY = oldY[step].data[i] - currentY[step].data[i];
        if (diffY < 0)
        {
            diffY = -diffY;
        }
        if (diffY > max)
        {
            max = diffY;
        }
    }

    return max;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Averager::Tester::ProcessX(const uint16 *data, int step)
{
    Copy16to32(data, currentX[step].data);

    if (enumAve == 0)
    {
        return;
    }
    
    uint *ave = &dataX[step].data[0];

    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        ave[i] = (uint)(ave[i] - (ave[i] >> enumAve) + data[i]);
        x[i] = (uint16)(ave[i] >> enumAve);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Averager::Tester::ProcessY(const uint8 *data, int step)
{
    Copy8to16(data, currentY[step].data);

    static int count = 0;

    LOG_WRITE("%d разность = %d", count++, Difference(step));

    std::memcpy(oldX[step].data, currentX[step].data, TESTER_NUM_POINTS * 4);
    std::memcpy(oldY[step].data, currentY[step].data, TESTER_NUM_POINTS * 2);

    if (enumAve == 0)
    {
        return;
    }
    
    uint16 *ave = &dataY[step].data[0];

    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        ave[i] = (uint16)(ave[i] - (ave[i] >> enumAve) + data[i]);
        y[i] = (uint8)(ave[i] >> enumAve);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16 *Averager::Tester::X()
{
    return x;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Averager::Tester::Y()
{
    return y;
}
