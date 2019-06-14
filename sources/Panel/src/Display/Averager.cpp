#include "defines.h"
#include "Log.h"
#include "Command.h"
#include "Averager.h"
#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int enumAve = 0;

struct StructX
{
    /// Усреднённые данные канала X
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

struct StructAVE
{
    StructX x;
    StructY y;
};

struct StructDATA
{
    uint16 x[TESTER_NUM_POINTS];
    uint8 y[TESTER_NUM_POINTS];
};

/// Это отрисовываемые данные
static StructDATA data      __attribute__((section("CCM_DATA")));

static StructAVE ave[5]     __attribute__((section("CCM_DATA")));

static StructAVE old[5]     __attribute__((section("CCM_DATA")));

static StructAVE current[5] __attribute__((section("CCM_DATA")));


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Copy16to32(const uint16 *_data, uint *buffer)
{
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        buffer[i] = _data[i];
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Copy8to16(const uint8 *_data, uint16 *buffer)
{
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        buffer[i] = _data[i];
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
                ave[step].x.data[i] = 0;
                ave[step].y.data[i] = 0;
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
        int diffX = (int)old[step].x.data[i] - (int)current[step].x.data[i];
        if (diffX < 0)
        {
            diffX = -diffX;
        }
        if (diffX > max)
        {
            max = diffX;
        }

        int diffY = old[step].y.data[i] - current[step].y.data[i];
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
void Averager::Tester::Process(const uint16 *dataX, const uint8 *dataY, int step)
{
    Copy16to32(dataX, current[step].x.data);

    if (enumAve == 0)
    {
        return;
    }

    uint *ave32 = &ave[step].x.data[0];

    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        ave32[i] = (uint)(ave32[i] - (ave32[i] >> enumAve) + dataX[i]);
        data.x[i] = (uint16)(ave32[i] >> enumAve);
    }

    Copy8to16(dataY, current[step].y.data);

    static int count = 0;

    LOG_WRITE("%d разность = %d", count++, Difference(step));

    std::memcpy(old[step].x.data, current[step].x.data, TESTER_NUM_POINTS * 4);
    std::memcpy(old[step].y.data, current[step].y.data, TESTER_NUM_POINTS * 2);

    if (enumAve == 0)
    {
        return;
    }
    
    uint16 *ave16 = &ave[step].y.data[0];

    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        ave16[i] = (uint16)(ave16[i] - (ave16[i] >> enumAve) + dataY[i]);
        data.y[i] = (uint8)(ave16[i] >> enumAve);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16 *Averager::Tester::X()
{
    return data.x;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Averager::Tester::Y()
{
    return data.y;
}
