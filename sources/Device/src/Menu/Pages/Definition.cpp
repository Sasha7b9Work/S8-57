#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Display_Primitives.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DrawSB_Exit(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Display::Primitives::Char('\x2e').Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void OnPressSB_Exit()
{
    Display::RemoveAddDrawFunction();
}
