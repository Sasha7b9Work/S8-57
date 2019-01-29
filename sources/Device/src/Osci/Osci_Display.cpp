#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Osci/Osci_Display.h"
#include "Osci/Display/BottomPart.h"
#include "Osci/Display/HiPart.h"
#include "Osci/Display/PainterData.h"
#include "Osci/Measurements/Measures.h"
#include "Osci/Measurements/Measurements_Table.h"
#include "Settings/Settings.h"


using namespace Display::Primitives;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DELTA 5


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Display::Update()
{
    Painter::BeginScene(Color::BACK);

    Grid::Draw();

    PainterData::DrawData();

    Rectangle(Grid::Width(), Grid::Height()).Draw(Grid::Left(), Grid::Top(), Color::FILL);

    BottomPart::Draw();

    RShift::DrawBoth();

    DrawCursorTrigLevel();

    Osci::Measurements::Cursors::Cursor::Draw();

    HiPart::Draw();

    FrequencyCounter::Draw();

    Menu::Draw();

    Measurements::Table::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Display::DrawCursorTrigLevel()
{
    Chan::E ch = (Chan::E)TRIG_SOURCE;

    int trigLev = SET_TRIGLEV_SOURCE + (SET_RSHIFT(ch) - RShift::ZERO);
    float scale = 1.0F / ((Trig::MAX - Trig::MIN) / 2.4F / Grid::Height());
    int y0 = (Grid::Top() + Grid::Bottom()) / 2 + (int)(scale * (Trig::ZERO - Trig::MIN));
    int y = y0 - (int)(scale * (trigLev - Trig::MIN));

    y = (y - Grid::ChannelCenterHeight()) + Grid::ChannelCenterHeight();

    int x = Grid::Right();
    Color::SetCurrent(Color::Trig());

    if (y > Grid::Bottom())
    {
        Char(SYMBOL_TRIG_LEV_LOWER).Draw(x + 3, Grid::Bottom() - 11);
        Point().Draw(x + 5, Grid::Bottom() - 2);
        y = Grid::Bottom() - 7;
        x--;
    }
    else if (y < Grid::Top())
    {
        Char(SYMBOL_TRIG_LEV_ABOVE).Draw(x + 3, Grid::Top() + 2);
        Point().Draw(x + 5, Grid::Top() + 2);
    }
    else
    {
        Char(SYMBOL_TRIG_LEV_NORMAL).Draw(x + 1, y - 4);
    }

    Font::SetCurrent(Font::Type::_5);

    static const char symbols[2] = {'1', '2'};

    Char(symbols[(uint8)TRIG_SOURCE]).Draw(x + 5, y - 6, Color::BACK);

    Font::SetCurrent(Font::Type::_8);

    DrawScaleLine(::Display::WIDTH - 11, true);
    int left = Grid::Right() + 9;
    int height = Grid::Height() - 2 * DELTA;
    int shiftFullMin = RShift::MIN + Trig::MIN;
    int shiftFullMax = RShift::MAX + Trig::MAX;
    scale = (float)height / (shiftFullMax - shiftFullMin);
    int shiftFull = SET_TRIGLEV_SOURCE + SET_RSHIFT(ch);
    int yFull = Grid::Top() + DELTA + height - (int)(scale * (shiftFull - RShift::MIN - Trig::MIN) + 4);
    Region(4, 6).Fill(left + 2, yFull + 1, Color::Trig());
    Font::SetCurrent(Font::Type::_5);
    Char(symbols[(uint8)TRIG_SOURCE]).Draw(left + 3, yFull - 2, Color::BACK);
    Font::SetCurrent(Font::Type::_8);

    Trig::DrawOnGrid();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Display::DrawScaleLine(int x, bool forTrigLev)
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
        Line(x + 1, levels[i], x2 - 1, levels[i]).Draw(Color::FILL);
    }
}
