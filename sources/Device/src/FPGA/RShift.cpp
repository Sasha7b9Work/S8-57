#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Grid.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGAMath.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#endif


using Display::Char;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RShift::Draw()
{
    Draw(Chan::A);
    Draw(Chan::B);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RShift::Draw(Chan::E ch)
{
    Color::SetCurrent(Color::Channel(ch));

    int delta = (SET_RSHIFT(ch) - RShift::ZERO) / STEP_RSHIFT;

    int y = (Grid::Bottom() - Grid::Top()) / 2 + Grid::Top() - delta;

    //Painter::DrawChar(Grid::Left() - 8, y - 4, (char)SYMBOL_RSHIFT_NORMAL);
    Char((char)SYMBOL_RSHIFT_NORMAL).Draw(Grid::Left() - 8, y - 4);

    Font::SetCurrent(Font::Type::_5);

    //Painter::DrawChar(Grid::Left() - 7, y - 6, Chan(ch).IsA() ? '1' : '2', Color::BACK);
    Char(Chan(ch).IsA() ? '1' : '2').Draw(Grid::Left() - 7, y - 6, Color::BACK);

    Font::SetCurrent(Font::Type::_8);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String RShift::ToString(uint16 rShiftRel, FPGA::SET::Range::E range, Divider::E divider)
{
    float rShiftVal = FPGAMath::RShift2Abs(rShiftRel, range) * Divider(divider).ToAbs();
    return Voltage(rShiftVal).ToString(true);
}
