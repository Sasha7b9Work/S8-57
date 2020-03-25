#include "defines.h"
#include "common/Display/Font/Font_d.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Display/Warnings.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Battery.h"
#include "Hardware/VCP.h"
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
    DFont::SetSpacing(1);

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

    TShift::Draw();

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


void DisplayOsci::BottomPart::Draw(int x0, int y0)
{
    DFont::Set(DTypeFont::_UGO2);

    // Флешка
    if(FDrive::IsConnected())
    {
    }

    if(VCP::connectedToUSB || VCP::cableUSBisConnected)
    {
        Char(SymbolUGO2::USB).Draw4SymbolsInRect(x0 + 72, y0 + 2, VCP::connectedToUSB ? Color::WHITE : Color::FLASH_01);
    }

    Color::FILL.SetAsCurrent();
    // Пиковый детектор
    if(PeakDetMode().IsEnabled())
    {
        Char('\x12').Draw(x0 + 38, y0 + 11);
        Char('\x13').Draw(x0 + 46, y0 + 11);
    }

    Battery::Draw(x0, y0);

    VLine(18).Draw(x0, y0 + 1, Color::FILL);
}
