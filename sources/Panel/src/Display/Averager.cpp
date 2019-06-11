#include "defines.h"
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

static StructX dataX[5] __attribute__((section("CCM_DATA")));
static StructY dataY[5] __attribute__((section("CCM_DATA")));


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Averager::Tester::SetCount(int count)
{
    enumAve = count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Averager::Tester::SetDataX(uint16 *data, int step)
{
    if (enumAve == 0)
    {
        return;
    }

    uint *ave = &dataX[step].data[0];

    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        ave[i] = (uint)(ave[i] - (ave[i] >> enumAve) + *data);
        *data = (uint16)(ave[i] >> enumAve);
        data++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Averager::Tester::SetDataY(uint8 *data, int step)
{
    if (enumAve == 0)
    {
        return;
    }

    uint16 *ave = &dataY[step].data[0];

    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        ave[i] = (uint16)(ave[i] - (ave[i] >> enumAve) + *data);
        *data = (uint8)(ave[i] >> enumAve);
        data++;
    }
}
