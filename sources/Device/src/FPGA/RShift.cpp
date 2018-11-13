#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGAMath.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RShift::Draw()
{
    Draw(Chan::A);
    Draw(Chan::B);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RShift::Draw(Chan ch)
{
    Painter::SetColor(Color::Channel(ch));

    int delta = (SET_RSHIFT(ch) - RShift::ZERO) / STEP_RSHIFT;

    int y = (Grid::Bottom() - Grid::Top()) / 2 + Grid::Top() - delta;

    Painter::DrawChar(Grid::Left() - 8, y - 4, (char)SYMBOL_RSHIFT_MARKER);

    Painter::SetFont(Font::Type::_5);

    Painter::DrawChar(Grid::Left() - 7, y - 6, ch.IsA() ? '1' : '2', Color::BACK);

    Painter::SetFont(Font::Type::_8);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString RShift::ToString(uint16 rShiftRel, Range range ,Divider divider, char buffer[20])
{
    float rShiftVal = MathFPGA::RShift2Abs(rShiftRel, range) * divider.ToAbs();
    return Voltage(rShiftVal).ToString(true, buffer);
}
