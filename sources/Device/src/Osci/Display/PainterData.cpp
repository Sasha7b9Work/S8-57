#include "defines.h"
#include "PainterData.h"
#include "Data/Reader.h"
#include "Display/Colors.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/PainterMem.h"
#include "Display/Symbols.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "MemoryWindow.h"

#include "Osci/Measurements/Measurements.h"


using namespace Display::Primitives;
using namespace FPGA;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Нарисовать актуальные данные - соответствующие текущим установкам
static void DrawCurrent();
/// Нарисовать данные из ОЗУ
static void DrawRAM();
/// Нарисовать данные из ППЗУ
static void DrawROM();

static void DrawChannel(Chan::E ch);

static void DrawTPos(int leftX, int rightX);

static void DrawTShift(int leftX, int rightX, int numPoints);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Display::PainterData::DrawData()
{
    if(Reader::ReadDataFromStorage())
    {
        Measurements::SetData(false);

        static const pFuncVV func[ModeWork::Number] =
        {
            DrawCurrent,
            DrawRAM,
            DrawROM
        };

        func[MODE_WORK]();
    }

    DrawTPos(0, 0);
    DrawTShift(0, 0, 0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawCurrent()
{
    if(LAST_AFFECTED_CH_IS_A)
    {
        DrawChannel(Chan::B);
        DrawChannel(Chan::A);
    }
    else
    {
        DrawChannel(Chan::A);
        DrawChannel(Chan::B);
    }

    Osci::Display::MemoryWindow::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawRAM()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawROM()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawChannel(Chan::E ch)
{
    uint8 *data = OUT(ch);

    if(SET_PEAKDET_DIS)
    {
        data += SHIFT_IN_MEMORY;
    }

    int center = (Grid::Bottom() - Grid::Top()) / 2 + Grid::Top();
    int left = Grid::Left();

    if (!SET_ENABLED(ch))
    {
        return;
    }
    
    float scale = (float)Grid::Height() / (VALUE::MAX - VALUE::MIN);

    if (MODE_DRAW_SIGNAL_IS_LINES)
    {
        if (THICKNESS_SIGNAL_IS_3)
        {
            Color::SetCurrent(Color::ChanHalf(ch));

            int x = left;

            for (int i = 1; i < 281; i++)
            {
                int value = (int)(center - ((data[i] - VALUE::AVE) * scale) + 0.5F);
                int valuePrev = (int)(center - ((data[i - 1] - VALUE::AVE) * scale) + 0.5F);

                if (value == valuePrev)
                {
                    HLine(2).Draw(x - 1, value);
                    VLine(2).Draw(x++, value - 1);
                }
                else
                {
                    if(valuePrev > value)   { ++value;  }
                    else                    { --value;  }

                    if(valuePrev < value)
                    {
                        ::Math::Swap(&valuePrev, &value);
                    }

                    VLine(valuePrev - value).Draw(x - 1, value);
                    VLine(valuePrev - value).Draw(x + 1, value);
                    VLine(value - valuePrev - 2).Draw(x++, valuePrev + 1);
                }
            }
        }

        Color::SetCurrent(Color::Channel(ch));

        int x = left;
         
        if(SET_PEAKDET_EN)
        {
            for(int i = 0; i < 281 * 2; i += 2)
            {
                int min = (int)(center - (data[i] - VALUE::AVE) * scale + 0.5F);
                int max = (int)(center - (data[i + 1] - VALUE::AVE) * scale + 0.5F);

                VLine(min - max).Draw(x++, max);
            }
        }
        else
        {
            for (int i = 1; i < 281; i++)
            {
                int value = (int)(center - (data[i] - VALUE::AVE) * scale + 0.5F);
                int valuePrev = (int)(center - (data[i - 1] - VALUE::AVE) * scale + 0.5F);

                if(value == valuePrev)
                {
                    Point().Draw(x++, valuePrev);
                }
                else
                {
                    int val = valuePrev > value ? (value + 1) : (value - 1);
                    VLine(val - valuePrev).Draw(x++, valuePrev);
                }
            }
        }
    }
    else
    {
        Color::SetCurrent(Color::Channel(ch));

        if(SET_PEAKDET_EN)
        {
            int x = left;
            for(int i = 0; i < 281 * 2; i += 2)
            {
                Point().Draw(x, (int)(center - (data[i] - VALUE::AVE) * scale + 0.5F));
                Point().Draw(x, (int)(center - (data[i + 1] - VALUE::AVE) * scale + 0.5F));
                x++;
            }
        }
        else
        {
            for (int i = 0; i < 280; i++)
            {
                float value = center - (data[i] - VALUE::AVE) * scale;
                Point().Draw(left + i, ROUND(uint8, value));
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTPos(int leftX, int rightX)
{
    int x[] = {leftX, (rightX - leftX) / 2 + leftX, rightX};
    int x0 = x[TPOS];
    Region(6, 6).Fill(x0 - 3, 10, Color::BACK);
    Char(SYMBOL_TPOS_1).Draw(x0 - 3, 10, Color::FILL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTShift(int leftX, int rightX, int numBytes)
{
    float scale = (float)(rightX - leftX + 1) / ((float)numBytes - (numBytes == 281 ? 1 : 0));
    int xShift = (int)(1.5F + (TPos(TPOS).InBytes() - SET_TSHIFT.InPoints()) * scale) - 1;
    if (SET_PEAKDET_EN && TPOS_IS_RIGHT)
    {
        --xShift;
    }
    if (FPGA_POINTS_512)
    {
        ++xShift;                           /// \todo Костыль
    }
    LIMIT_ABOVE(xShift, rightX - 2);

    int dX01 = 1, dX02 = 2, dX11 = 3, dY11 = 7, dY12 = 6;

    if (xShift < leftX - 2)
    {
        xShift = leftX - 2;
        dX01 = 3; dX02 = 1; dY12 = 6;
    }
    else if (xShift > rightX - 1)
    {
        xShift = rightX - 2;
        dX11 = 1;
    }
    else
    {
        dY11 = 5; dY12 = 7;
    }

    Region(6, 6).Fill((int)xShift - 1, 1, Color::BACK);
    Region(4, 4).Fill((int)xShift, 2, Color::FILL);

    Line((int)xShift + dX01, 3, (int)xShift + dX11, dY11 - 2).Draw(Color::BACK);
    Line((int)xShift + dX02, 4, (int)xShift + 2, dY12 - 2).Draw();
}
