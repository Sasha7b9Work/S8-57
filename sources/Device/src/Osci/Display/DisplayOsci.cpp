#include "defines.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Display/Warnings.h"
#include "Menu/Menu.h"
#include "Osci/Osci.h"
#include "Osci/FreqMeter.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Osci/Measurements/CursorsMeasurements.h"
#include "Settings/Settings.h"


static const int DELTA = 5;
/// Признак того, что дисплей нуждается в полной перерисовке
static bool needRedraw = false;



void DisplayOsci::Update()
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

    AveragerOsci::Draw();
    
    RShift::DrawBoth();

    DrawCursorTrigLevel();
    
    CursorsMeasurements::Draw();

    BottomPart::Draw(273, Grid::Bottom() + 1);
    
    FrequencyCounter::Draw();

    TableMeasures::Draw();

    Menu::Draw();

    Warnings::Draw();
}


void DisplayOsci::DrawCursorTrigLevel()
{
    Chan::E ch = static_cast<Chan::E>(set.trig.source);

    int trigLev = set.trig.lev[set.trig.source] + SET_RSHIFT(ch);
    float scale = 1.0F / ((TrigLevel::MAX - TrigLevel::MIN) / 2.4F / Grid::Height());
    int y0 = (Grid::Top() + Grid::ChannelBottom()) / 2 + static_cast<int>(scale * (TrigLevel::HARDWARE_ZERO - TrigLevel::MIN));
    int y = y0 - static_cast<int>(scale * (trigLev - TrigLevel::MIN));

    y = (y - Grid::ChannelCenterHeight()) + Grid::ChannelCenterHeight();

    int x = Grid::Right();
    Color::Trig().SetAsCurrent();

    if (y > Grid::ChannelBottom())
    {
        Char(Symbol8::TRIG_LEV_LOWER).Draw(x + 3, Grid::ChannelBottom() - 11);
        Pixel().Draw(x + 5, Grid::ChannelBottom() - 2);
        y = Grid::ChannelBottom() - 7;
        x--;
    }
    else if (y < Grid::Top())
    {
        Char(Symbol8::TRIG_LEV_ABOVE).Draw(x + 3, Grid::Top() + 2);
        Pixel().Draw(x + 5, Grid::Top() + 2);
    }
    else
    {
        Char(Symbol8::TRIG_LEV_NORMAL).Draw(x + 1, y - 4);
    }

    static const char symbols[2] = {'1', '2'};

    Char(symbols[static_cast<uint8>(set.trig.source)], TypeFont::_5).Draw(x + 5, y - 6, Color::BACK);

    Trig::DrawOnGrid();
}


void DisplayOsci::DrawScaleLine(int x, bool forTrigLev)
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


void DisplayOsci::SetFlagRedraw()
{
    needRedraw = true;
}
