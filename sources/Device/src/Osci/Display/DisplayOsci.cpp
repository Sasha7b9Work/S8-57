#include "defines.h"
#include "common/Display/Font/Font_d.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Display/Warnings.h"
#include "FlashDrive/FlashDrive.h"
#include "FreqMeter/DisplayFreqMeter.h"
#include "Hardware/Battery.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include "Menu/Menu.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Osci/Measurements/CursorsMeasurements.h"
#include "Settings/Settings.h"
#include "Utils/Values.h"


bool DisplayOsci::needRedraw = false;
bool DisplayOsci::DrawingValueParameter::needDrawParameter = false;
DisplayOsci::DrawingValueParameter::E DisplayOsci::DrawingValueParameter::parameter;


static const int DELTA = 5;



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

    TPos::Draw();

    TShift::Draw();

    RShift::DrawBoth();
    
    TrigLevel::Draw();

    DrawingValueParameter::Draw();
    
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


void DisplayOsci::DrawingValueParameter::Enable(DrawingValueParameter::E v)
{
    if(S_FFT_ENABLED)                                                                     // Не будем рисовать при включённом спектре
    {
        return;
    }

    if (v == DrawingValueParameter::TrigLevel)
    {
        if (S_TRIG_MODE_FIND_IS_AUTO)
        {
            return;
        }
    }
    else if(!CursorsMeasurements::NecessaryDraw())
    {
//        return;
    }

    needDrawParameter = true;

    parameter = v;

    Timer::SetAndStartOnce(TypeTimer::ShowLevelTrigLev, Disable, 2000);

    DisplayOsci::SetFlagRedraw();
}


void DisplayOsci::DrawingValueParameter::Draw()
{
    if(needDrawParameter)
    {
        int y = Grid::ChannelBottom() - 33;

        switch (parameter)
        {
        case TrigLevel:
        {
            float trigLevVal = RShift::ToAbs(S_TRIG_LEVEL_SOURCE, S_RANGE(S_TRIG_SOURCE)) * Divider::ToAbs(S_DIVIDER(S_TRIG_SOURCE));
            Voltage voltage(trigLevVal);
            String text("Синхр %s", voltage.ToString(true).c_str());
            DrawBoundedText(y, &text, Color::CHAN[S_TRIG_SOURCE]);
            break;
        }

        case RangeA:
        case RangeB:
        case RShiftA:
        case RShiftB:
        {
            Chan::E ch = ((parameter == RangeA) || (parameter == RShiftA)) ? ChanA : ChanB;
            char *channels[2] = { "1", "2" };
            String text("%sк: %s %s", channels[ch], Range::ToString(ch, S_DIVIDER(ch)), RShift::ToString(S_RSHIFT(ch), S_RANGE(ch), S_DIVIDER(ch)).c_str());
            DrawBoundedText(y, &text, Color::CHAN[ch]);
            break;
        }

        case TBase:
        case TShift:
            String text("р: %s %s", TBase::ToString(S_TIME_BASE), TShift::ToString(S_TIME_BASE).c_str());
            DrawBoundedText(y, &text, Color::FILL);
            break;
        }
    }
}


void DisplayOsci::DrawingValueParameter::DrawBoundedText(int y, String *text, Color color)
{
    int length = DFont::GetLengthText(text->c_str()) + 6;
    int x = Grid::Left() + Grid::Width() / 2 - length / 2;
    Region(length, 16).DrawBounded(x, y, Color::BACK, Color::FILL);
    text->Draw(x + 4, y + 4, color);
}


void DisplayOsci::DrawingValueParameter::Disable()
{
    needDrawParameter = false;
}
