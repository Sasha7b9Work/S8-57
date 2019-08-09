#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"
#include "Display/Display_Primitives.h"
#include "Display/Display_Types.h"
#include "Settings/Settings.h"
#include "Utils/CommonFunctions.h"
#include "Device.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageService.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Menu/Pages/Include/PageTime.h"


using namespace Display::Primitives;
using namespace Osci::Measurements;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawSB_Exit(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x2e').Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void OnPressSB_Exit()
{
    Display::RemoveAddDrawFunction();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static int CalculateYforCurs(int y, bool top)
{
    return y + 18 + (top ? -3 : 3);
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
static int CalculateXforCurs(int x, bool left)
{
    return x + 52 + (left ? -5 : 5);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static void CalculateXY(int *x0, int *x1, int *y0, int *y1)
{
    *x0 = CalculateXforCurs(*x0, true);
    *x1 = CalculateXforCurs(*x1, false);
    *y0 = CalculateYforCurs(*y0, true);
    *y1 = CalculateYforCurs(*y1, false);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuCursVoltage(int x, int y, bool top, bool bottom)
{
    x -= 49;
    y -= 14;
    int x0 = x, x1 = x, yTop = y, yBottom = y;

    CalculateXY(&x0, &x1, &yTop, &yBottom);

	HLine line(x1 - x0);

    for (int i = 0; i < (top ? 3 : 1); i++)
    {
		line.Draw(x0, yTop + i);
    }

    for (int i = 0; i < (bottom ? 3 : 1); i++)
    {
		line.Draw(x0, yBottom - i);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CalculateConditions(int16 pos0, int16 pos1, Cursors::Control::E cursCntrl, bool *condTopLeft, bool *condBottomRight)
{
    bool zeroLessFirst = pos0 < pos1;
    *condTopLeft     =  (cursCntrl == Cursors::Control::_1_2) ||                    // если управление двумя курсорами одновременно
                        (cursCntrl == Cursors::Control::_1 && zeroLessFirst) ||     // или управление первым курсором и позиция первого меньше, чем позиция второго
                        (cursCntrl == Cursors::Control::_2 && !zeroLessFirst);      // или управление вторым курсором и позиция второго курсора меньше

    *condBottomRight =  (cursCntrl == Cursors::Control::_1_2) || 
                        (cursCntrl == Cursors::Control::_1 && !zeroLessFirst) || 
                        (cursCntrl == Cursors::Control::_2 && zeroLessFirst);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuCursTime(int x, int y, bool left, bool right)
{
    x -= 40;
    y -= 10;
    int x0 = x, x1 = x, y0 = y, y1 = y;
    CalculateXY(&x0, &x1, &y0, &y1);

    VLine line(y1 - y0);

    for (int i = 0; i < (left ? 3 : 1); i++)
    {
        line.Draw(x0 + i, y0);
    }

    for (int i = 0; i < (right ? 3 : 1); i++)
    {
        line.Draw(x1 - i, y0);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool EmptyFuncBKE(KeyEvent)
{
    return false;
}
