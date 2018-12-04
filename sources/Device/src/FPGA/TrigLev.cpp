#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FPGAMath.h"
#include "TrigLev.h"
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
/// Преобразует уровень синхронизации в текстовую строку
static String ToString(uint16 trigLev, Range::E range, Divider::E divider);
/// Отключает отображение уровня синхронизации поверх сигнала
static void DisableDrawing();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Trig::SyncPulse()
{
    return pulse;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static String ToString(uint16 trigLev, Range::E range, Divider::E divider)
{
    float trigLevVal = FPGAMath::RShift2Abs(trigLev, range) * Divider(divider).ToAbs();
    return Voltage(trigLevVal).ToString(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::DrawOnGrid()
{
    if(needDraw)
    {
        char buffer[20];
        sprintf(buffer, "Синхр %s", ToString(SET_TRIGLEV_SOURCE, SET_RANGE(TRIG_SOURCE), SET_DIVIDER(TRIG_SOURCE)).CString());

        int width = 85;
        int height = 18;

        int x = (Grid::Right() - Grid::Left()) / 2 + Grid::Left() - width / 2;
        int y = Grid::Bottom() - height - 20;

        Painter::FillBoundedRegion(x, y, width, height, Color::BACK, Color::FILL);
        Painter::DrawText(x + 7, y + 5, buffer, Color::FILL);
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
