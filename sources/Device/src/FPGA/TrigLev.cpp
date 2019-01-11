#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FPGAMath.h"
#include "TrigLev.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Values.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Trig::pulse = false;
///< Нужно ли рисовать горизонтальную линию уровня смещения уровня синхронизации.
volatile static bool needDraw = false; 



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Отключает отображение уровня синхронизации поверх сигнала
static void DisableDrawing();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Trig::SyncPulse()
{
    return pulse;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::DrawOnGrid()
{
    if(needDraw)
    {
        int width = 85;
        int height = 18;

        int x = (Grid::Right() - Grid::Left()) / 2 + Grid::Left() - width / 2;
        int y = Grid::Bottom() - height - 20;

        Painter::FillBoundedRegion(x, y, width, height, Color::BACK, Color::FILL);


        float trigLevVal = FPGAMath::RShift2Abs(SET_TRIGLEV_SOURCE, SET_RANGE(TRIG_SOURCE)) * Divider(SET_DIVIDER(TRIG_SOURCE)).ToAbs();

        Voltage voltage(trigLevVal);

        String("Синхр %s", voltage.ToString(true).CString()).Draw(x + 7, y + 5, Color::FILL);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::NeedForDraw(uint timeMS)
{
    needDraw = true;
    Timer::SetAndStartOnce(Timer::Type::ShowLevelTrigLev, DisableDrawing, timeMS);
    NEED_FINISH_DRAW = 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DisableDrawing()
{
    needDraw = false;
}
