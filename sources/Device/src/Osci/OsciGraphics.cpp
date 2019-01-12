#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Osci.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "BottomPart.h"
#include "HiPart.h"
#include "PainterData.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGATypes.h"
#include "FPGA/RShift.h"
#include "FrequencyCounter.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using Display::Rectangle;
using Display::Region;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DELTA 5


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Draw()
{
    Painter::BeginScene(Color::BACK);

    Grid::Draw();
    
    PainterData::DrawData();
    
    Rectangle(Grid::Width(), Grid::Height()).Draw(Grid::Left(), Grid::Top(), Color::FILL);
    
    BottomPart::Draw();
    
    RShift::Draw();
    
    DrawCursorTrigLevel();
    
    Cursors::Draw();
    
    HiPart::Draw();
    
    FrequencyCounter::Draw();

    Menu::Draw();

    Measure::Graphics::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::DrawCursorTrigLevel()
{
    Trig::Source::E ch = TRIG_SOURCE;

    int trigLev = SET_TRIGLEV_SOURCE - (TRIG_SOURCE_IS_EXT ? 0 : -(SET_RSHIFT(ch) - RShift::ZERO));
    float scale = 1.0F / ((Trig::MAX - Trig::MIN) / 2.4F / Grid::Height());
    int y0 = (Grid::Top() + Grid::Bottom()) / 2 + (int)(scale * (Trig::ZERO - Trig::MIN));
    int y = y0 - (int)(scale * (trigLev - Trig::MIN));

    if (!TRIG_SOURCE_IS_EXT)
    {
        y = (y - Grid::ChannelCenterHeight()) + Grid::ChannelCenterHeight();
    }

    int x = Grid::Right();
    Painter::SetColor(Color::Trig());

    if (y > Grid::Bottom())
    {
        Painter::DrawChar(x + 3, Grid::Bottom() - 11, SYMBOL_TRIG_LEV_LOWER);
        Painter::SetPoint(x + 5, Grid::Bottom() - 2);
        y = Grid::Bottom() - 7;
        x--;
    }
    else if (y < Grid::Top())
    {
        Painter::DrawChar(x + 3, Grid::Top() + 2, SYMBOL_TRIG_LEV_ABOVE);
        Painter::SetPoint(x + 5, Grid::Top() + 2);
    }
    else
    {
        Painter::DrawChar(x + 1, y - 4, SYMBOL_TRIG_LEV_NORMAL);
    }

    Painter::SetFont(Font::Type::_5);

    static const char symbols[2] = {'1', '2'};

    Painter::DrawChar(x + 5, y - 6, symbols[(uint8)TRIG_SOURCE], Color::BACK);
    Painter::SetFont(Font::Type::_8);

    DrawScaleLine(Display::WIDTH - 11, true);
    int left = Grid::Right() + 9;
    int height = Grid::Height() - 2 * DELTA;
    int shiftFullMin = RShift::MIN + Trig::MIN;
    int shiftFullMax = RShift::MAX + Trig::MAX;
    scale = (float)height / (shiftFullMax - shiftFullMin);
    int shiftFull = SET_TRIGLEV_SOURCE + (TRIG_SOURCE_IS_EXT ? 0 : SET_RSHIFT(ch));
    int yFull = Grid::Top() + DELTA + height - (int)(scale * (shiftFull - RShift::MIN - Trig::MIN) + 4);
    Region(4, 6).Draw(left + 2, yFull + 1, Color::Trig());
    Painter::SetFont(Font::Type::_5);
    Painter::DrawChar(left + 3, yFull - 2, symbols[(uint8)TRIG_SOURCE], Color::BACK);
    Painter::SetFont(Font::Type::_8);

    Trig::DrawOnGrid();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::DrawScaleLine(int x, bool forTrigLev)
{
    int width = 6;
    int topY = Grid::Top() + DELTA;
    int x2 = width + x + 2;
    int bottomY = Grid::Bottom() - DELTA;
    int centerY = (Grid::Bottom() + Grid::Top()) / 2;
    int levels[] =
    {
        topY,
        bottomY,
        centerY,
        centerY - (bottomY - topY) / (forTrigLev ? 8 : 4),
        centerY + (bottomY - topY) / (forTrigLev ? 8 : 4)
    };

    for (int i = 0; i < 5; i++)
    {
        //Painter::DrawLine(x + 1, levels[i], x2 - 1, levels[i], Color::FILL);
    }
}
