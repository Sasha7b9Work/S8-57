#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "PainterData.h"
#include "Data/DataStorage.h"
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
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using Display::Char;
using Display::HLine;
using Display::Line;
using Display::Point;
using Display::Region;
using Display::VLine;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StructDataDrawing *PainterData::dataStruct = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PainterData::DrawData()
{
    if(Reader::ReadData())
    {
        Measure::SetData(false);

        static const pFuncVV func[ModeWork::Number] =
        {
            PainterData::DrawCurrent,
            PainterData::DrawRAM,
            PainterData::DrawROM
        };

        func[MODE_WORK]();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawCurrent()
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

    MemoryWindow::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawRAM()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawROM()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawChannel(Chan::E ch)
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
    
    float scale = (float)Grid::Height() / (MAX_VALUE - MIN_VALUE);

    if (MODE_DRAW_SIGNAL_IS_LINES)
    {
        if (THICKNESS_SIGNAL_IS_3)
        {
            Color::SetCurrent(Color::ChanHalf(ch));

            int x = left;

            for (int i = 1; i < 281; i++)
            {
                int value = (int)(center - ((data[i] - AVE_VALUE) * scale) + 0.5F);
                int valuePrev = (int)(center - ((data[i - 1] - AVE_VALUE) * scale) + 0.5F);

                if (value == valuePrev)
                {
                    //Painter::DrawHLine(value, x - 1, x + 1);
                    HLine(2).Draw(x - 1, value);

                    //Painter::DrawVLine(x++, value - 1, value + 1);
                    VLine(2).Draw(x++, value - 1);
                }
                else
                {
                    if(valuePrev > value)   { ++value;  }
                    else                    { --value;  }

                    if(valuePrev < value)
                    {
                        Math::Swap(&valuePrev, &value);
                    }

                    //Painter::DrawVLine(x - 1, value, valuePrev);
                    VLine(valuePrev - value).Draw(x - 1, value);

                    //Painter::DrawVLine(x + 1, value, valuePrev);
                    VLine(valuePrev - value).Draw(x + 1, value);

                    //Painter::DrawVLine(x++, valuePrev + 1, value - 1);
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
                int min = (int)(center - (data[i] - AVE_VALUE) * scale + 0.5F);
                int max = (int)(center - (data[i + 1] - AVE_VALUE) * scale + 0.5F);

                //Painter::DrawVLine(x++, max, min);
                VLine(min - max).Draw(x++, max);
            }
        }
        else
        {
            for (int i = 1; i < 281; i++)
            {
                int value = (int)(center - (data[i] - AVE_VALUE) * scale + 0.5F);
                int valuePrev = (int)(center - (data[i - 1] - AVE_VALUE) * scale + 0.5F);

                if(value == valuePrev)
                {
                    //Painter::SetPoint(x++, valuePrev);
                    Point().Draw(x++, valuePrev);
                }
                else
                {
                    int val = valuePrev > value ? (value + 1) : (value - 1);
                    //Painter::DrawVLine(x++, valuePrev, val);
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
                //Painter::SetPoint(x, (int)(center - (data[i] - AVE_VALUE) * scale + 0.5F));
                Point().Draw(x, (int)(center - (data[i] - AVE_VALUE) * scale + 0.5F));
                //Painter::SetPoint(x, (int)(center - (data[i + 1] - AVE_VALUE) * scale + 0.5F));
                Point().Draw(x, (int)(center - (data[i + 1] - AVE_VALUE) * scale + 0.5F));
                x++;
            }
        }
        else
        {
            for (int i = 0; i < 280; i++)
            {
                float value = center - (data[i] - AVE_VALUE) * scale;
                //Painter::SetPoint(left + i, ROUND(uint8, value));
                Point().Draw(left + i, ROUND(uint8, value));
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawTPos(int leftX, int rightX)
{
    int x[] = {leftX, (rightX - leftX) / 2 + leftX, rightX};
    int x0 = x[TPOS];
    Region(6, 6).Draw(x0 - 3, 10, Color::BACK);
    //Painter::DrawChar(x0 - 3, 10, SYMBOL_TPOS_1, Color::FILL);
    Char(SYMBOL_TPOS_1).Draw(x0 - 3, 10, Color::FILL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::DrawTShift(int leftX, int rightX, int numBytes)
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

    Region(6, 6).Draw((int)xShift - 1, 1, Color::BACK);
    Region(4, 4).Draw((int)xShift, 2, Color::FILL);

    //Painter::DrawLine((int)xShift + dX01, 3, (int)xShift + dX11, dY11 - 2, Color::BACK);
    Line((int)xShift + dX01, 3, (int)xShift + dX11, dY11 - 2).Draw(Color::BACK);

    //Painter::DrawLine((int)xShift + dX02, 4, (int)xShift + 2, dY12 - 2);
    Line((int)xShift + dX02, 4, (int)xShift + 2, dY12 - 2).Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int PainterData::Ordinate(uint8 x, float scale)
{
    if (x == NONE_VALUE)
    {
        return -1;
    }

    Math::LimitationRet<uint8>((uint8)(x - MIN_VALUE), 0, (MAX_VALUE - MIN_VALUE));

    return (int)((17.0F - scale * x) + 0.5F);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PainterData::SendToDisplayDataInRect(Chan::E ch, int x, const int *min, const int *max, int width)
{
    LIMIT_ABOVE(width, 255);

#undef SIZE_BUFFER
#define SIZE_BUFFER (255 * 2)
    uint8 points[SIZE_BUFFER];

    for (int i = 0; i < width; i++)
    {
        points[i * 2] = (uint8)max[i];
        points[i * 2 + 1] = (uint8)(min[i] < 0 ? 0 : min[i]);
    }

    Painter::DrawVLineArray(x, (int)width, points, Color::Channel(ch));
}
