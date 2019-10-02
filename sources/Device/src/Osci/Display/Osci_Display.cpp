#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Osci/Display/HiPart.h"
#include "Osci/Display/BottomPart.h"
#include "Osci/Display/Osci_Display.h"
#include "Osci/Display/PainterData.h"
#include "Osci/Measurements/Measures.h"
#include "Osci/Measurements/Measurements_Table.h"
#include "Osci/Measurements/Measurements.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"
#include "Osci/Display/Accumulator.h"
#include "Osci/Osci_Averager.h"


using namespace Display::Primitives;
using namespace Osci::Measurements;



static const int DELTA = 5;
/// Признак того, что дисплей нуждается в полной перерисовке
static bool needRedraw = false;



void Osci::Display::Update()
{
    if (needRedraw)
    {
        Painter::BeginScene(Color::BACK);

        Grid::Draw();

        needRedraw = false;

        Accumulator::Reset();
    }

    PainterData::DrawData();

    Rectangle(Grid::Width(), Grid::Height()).Draw(Grid::Left(), Grid::Top(), Color::FILL);

    /// \todo Говнокод. Этого здесь быть не должно, но иначе напряжение в параметрах каналов выводится малыми буквами
    Font::SetCurrent(TypeFont::_8);

    HiPart::Draw();

    Averager::Draw();
    
    RShift::DrawBoth();

    DrawCursorTrigLevel();
    
    Osci::Measurements::Cursors::Draw();

    BottomPart::Draw(273, Grid::Bottom() + 1);
    
    FrequencyCounter::Draw();

    Measurements::Table::Draw();

    Menu::Draw();
}


void Osci::Display::DrawCursorTrigLevel()
{
    Chan::E ch = (Chan::E)set.trig.source;

    int trigLev = set.trig.lev[set.trig.source] + (SET_RSHIFT(ch) - RShift::ZERO);
    float scale = 1.0F / ((Trig::Level::MAX - Trig::Level::MIN) / 2.4F / Grid::Height());
    int y0 = (Grid::Top() + Grid::ChannelBottom()) / 2 + (int)(scale * (Trig::Level::ZERO - Trig::Level::MIN));
    int y = y0 - (int)(scale * (trigLev - Trig::Level::MIN));

    y = (y - Grid::ChannelCenterHeight()) + Grid::ChannelCenterHeight();

    int x = Grid::Right();
    Color::Trig().SetAsCurrent();

    if (y > Grid::ChannelBottom())
    {
        Char(Symbol8::TRIG_LEV_LOWER).Draw(x + 3, Grid::ChannelBottom() - 11);
        Point().Draw(x + 5, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        x--;
    }
    else if (y < Grid::Top())
    {
        Char(Symbol8::TRIG_LEV_ABOVE).Draw(x + 3, Grid::Top() + 2);
        Point().Draw(x + 5, Grid::Top() + 2);
    }
    else
    {
        Char(Symbol8::TRIG_LEV_NORMAL).Draw(x + 1, y - 4);
    }

    Font::SetCurrent(TypeFont::_5);

    static const char symbols[2] = {'1', '2'};

    Char(symbols[(uint8)set.trig.source]).Draw(x + 5, y - 6, Color::BACK);

    Font::SetCurrent(TypeFont::_8);

    Trig::DrawOnGrid();
}


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


void Osci::Display::SetFlagRedraw()
{
    needRedraw = true;
}
