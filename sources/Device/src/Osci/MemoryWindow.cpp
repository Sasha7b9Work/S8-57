#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "MemoryWindow.h"
//#include "rid.h"
#include "Display/Painter.h"
#include "Data/DataStorage.h"
#include "Data/Reader.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MemoryWindow::Draw()
{
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
void MemoryWindow::DrawDataInRect(int x, int y, int width, int height, uint8 *data, uint length)
{
    int numIntervals = width + 1;            //  оличество интервалов, в которых будем рисовать наш сигнал - фактически, количество вертикальных линий
    float pointsInInterval = (float)length / numIntervals;   //  оличество точек, рисуемых в одном интервале.

    float stepY = (float)height / (float)(MAX_VALUE - MIN_VALUE);

    int y0 = y + height;

    for (int i = 0; i < numIntervals - 1; i++)
    {
        int start = (int)(i * pointsInInterval + 0.5f);          // Ќачальна€ точка в интервале
        int end = (int)((i + 1) * pointsInInterval + 0.5f) - 1;  //  онечна€ точка в интервале

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

        Painter::DrawVLine(x + i, y0 - (int)(min * stepY + 0.5f), y0 - (int)(max * stepY + 0.5f));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MemoryWindow::DrawScreenArea()
{
    float relBegin = (float)SHIFT_IN_MEMORY / FPGA::NumPoints();          // ќтносительное начало экранной области относительно всей пам€ти
    float relWidth = (float)(Grid::Width() + 1) / FPGA::NumPoints();      // ќтносительное (относительно всей шириный окна) ширина видимой области

    int begin = (int)(relBegin * Width() + 0.5f);   // –еальное начало экранной области относительно начала окна пам€ти
    int width = (int)(relWidth * Width() + 0.5f);   // –еальна€ ширина видимой области

    Painter::FillRegion(X() + begin, Y(), width, Height(), Color::GRAY_20);

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
