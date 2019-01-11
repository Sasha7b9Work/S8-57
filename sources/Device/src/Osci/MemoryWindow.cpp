#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "MemoryWindow.h"
#include "Data/DataStorage.h"
#include "Data/Reader.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using Display::Region;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MemoryWindow::Draw()
{
    if (Cursors::NecessaryDraw())
    {
        return;
    }

    DrawScreenArea();

    Chan::E chans[2] = {LAST_AFFECTED_CH_IS_A ? Chan::B : Chan::A, LAST_AFFECTED_CH_IS_A ? Chan::A : Chan::B};

    for (int i = 0; i < 2; i++)
    {
        Chan::E chan = chans[i];
        if (SET_ENABLED(chan))
        {
            Painter::SetColor(Color::Channel(chan));
            DrawDataInRect(X(), Y(), Width(), Height(), OUT(chan), (uint)FPGA::NumPoints());
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryWindow::DrawDataInRect(int x, int y, int width, int height, const uint8 *data, uint length)
{
    int numIntervals = width + 1;            //  оличество интервалов, в которых будем рисовать наш сигнал - фактически, количество вертикальных линий
    float pointsInInterval = (float)length / numIntervals;   //  оличество точек, рисуемых в одном интервале.

    float stepY = (float)height / (float)(MAX_VALUE - MIN_VALUE);

    int y0 = y + height;

    for (int i = 0; i < numIntervals - 1; i++)
    {
        int start = (int)(i * pointsInInterval + 0.5F);          // Ќачальна€ точка в интервале
        int end = (int)((i + 1) * pointsInInterval + 0.5F) - 1;  //  онечна€ точка в интервале

        int min = 255;
        int max = 0;

        for (int j = start; j <= end; j++)
        {
            if (data[j] < min)
            {
                min = data[j];
            }
            if (data[j] > max)
            {
                max = data[j];
            }
        }

        Painter::DrawVLine(x + i, y0 - (int)(min * stepY + 0.5F), y0 - (int)(max * stepY + 0.5F));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryWindow::DrawScreenArea()
{
    float relBegin = (float)SHIFT_IN_MEMORY / FPGA::NumPoints();          // ќтносительное начало экранной области относительно всей пам€ти
    float relWidth = (float)(Grid::Width() + 1) / FPGA::NumPoints();      // ќтносительное (относительно всей шириный окна) ширина видимой области

    int begin = (int)(relBegin * Width() + 0.5F);   // –еальное начало экранной области относительно начала окна пам€ти
    int width = (int)(relWidth * Width() + 0.5F);   // –еальна€ ширина видимой области

    Region(width, Height()).Draw(X() + begin, Y(), Color::GRAY_20);

    Painter::DrawRectangle(X() + begin, Y(), width, Height(), Color::FILL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int MemoryWindow::Width()
{
    return 278;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int MemoryWindow::Height()
{
    return Grid::Top() - 2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int MemoryWindow::X()
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int MemoryWindow::Y()
{
    return 0;
}
