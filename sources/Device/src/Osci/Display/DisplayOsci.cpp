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

    Rectangle(Grid::Width(), Grid::ChannelHeight()).Draw(Grid::Left(), Grid::Top(), Color::FILL);

    HiPart::Draw();

    TPos().Draw();

    TShift().Draw();

    RShift::DrawBoth();
    
    TrigLevel().Draw();
    
    CursorsMeasurements::Draw();
    
    BottomPart::Draw(273, Grid::Bottom() + 1);
    
    DisplayFreqMeter::Draw();
    
    TableMeasures::Draw();
    
    Menu::Draw();

    Warnings::Draw();
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
