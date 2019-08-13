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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawMenuCursVoltage(int x, int y, bool top, bool bottom)
{
    x -= 10;
    y += 6;

	HLine line(40);

    for (int i = 0; i < (top ? 3 : 1); i++)
    {
		line.Draw(x, y + i);
    }

    for (int i = 0; i < (bottom ? 3 : 1); i++)
    {
		line.Draw(x, y + 8 - i);
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
    x += 11;
    y += 1;

    VLine line(15);

    for (int i = 0; i < (left ? 3 : 1); i++)
    {
        line.Draw(x - 15 + i, y + 1);
    }

    for (int i = 0; i < (right ? 3 : 1); i++)
    {
        line.Draw(x + 15 - i, y + 1);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool EmptyFuncBKE(KeyEvent)
{
    return false;
}
