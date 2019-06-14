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

/// Это отрисовываемые данные - после усреднения
static StructDATA data      __attribute__((section("CCM_DATA")));
/// Массив накопленных усреднений
static StructAVE ave[5]     __attribute__((section("CCM_DATA")));
/// Предыдущие поданные данные
static StructDATA old[5]     __attribute__((section("CCM_DATA")));
/// Текущие поданные данные
static StructDATA current[5] __attribute__((section("CCM_DATA")));


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Copy16(uint16 *dest, const uint16 *src)
{
    std::memcpy(dest, src, TESTER_NUM_POINTS * 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Copy8(uint8 *dest, const uint8 *src)
{
    std::memcpy(dest, src, TESTER_NUM_POINTS);
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
void Averager::Tester::Process(const uint16 *dataX, const uint8 *dataY, int step)
{
    Copy16(current[step].x, dataX);
    Copy8(current[step].y, dataY);

    if (enumAve == 0)
    {
        Copy16(data.x, dataX);
        Copy8(data.y, dataY);

        return;
    }

    uint *ave32 = &ave[step].x.data[0];

    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        ave32[i] = (uint)(ave32[i] - (ave32[i] >> enumAve) + dataX[i]);
        data.x[i] = (uint16)(ave32[i] >> enumAve);
    }

    Copy16(old[step].x, current[step].x);
    Copy8(old[step].y, current[step].y);
    
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
