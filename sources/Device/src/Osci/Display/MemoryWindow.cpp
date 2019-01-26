#include "defines.h"
#include "MemoryWindow.h"
#include "Data/DataStorage.h"
#include "Data/Reader.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"

#include "Hardware/Battery.h"
#include "Osci/Display/BottomPart.h"
#include <stm32f4xx_hal.h>


using namespace Display::Primitives;
using namespace FPGA;
using namespace Osci::Measurements::Cursors;

using Osci::Display::MemoryWindow::Width;
using Osci::Display::MemoryWindow::Height;
using Osci::Display::MemoryWindow::X;
using Osci::Display::MemoryWindow::Y;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawDataInRect(int x, int y, int width, int height, const uint8 *data, uint length);
/// Нарисовать область экрана
static void DrawScreenArea();

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Display::MemoryWindow::Draw()
{
    if (Cursor::NecessaryDraw())
    {
        return;
    }

    if (Menu::IsShown())
    {
        BottomPart::DrawMainParameters(0, 1);
    }
    else
    {
        DrawScreenArea();

        Chan::E chans[2] = { LAST_AFFECTED_CH_IS_A ? Chan::B : Chan::A, LAST_AFFECTED_CH_IS_A ? Chan::A : Chan::B };

        for (int i = 0; i < 2; i++)
        {
            Chan::E chan = chans[i];
            if (SET_ENABLED(chan))
            {
                Color::SetCurrent(Color::Channel(chan));
                DrawDataInRect(X(), Y(), Width(), Height(), OUT(chan), (uint)FPGA::NumPoints());
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDataInRect(int x, int y, int width, int height, const uint8 *data, uint length)
{
    int numIntervals = width + 1;            // Количество интервалов, в которых будем рисовать наш сигнал - фактически, количество вертикальных линий
    float pointsInInterval = (float)length / numIntervals;   // Количество точек, рисуемых в одном интервале.

    float stepY = (float)height / (float)(VALUE::MAX - VALUE::MIN);

    int y0 = y + height;

    for (int i = 0; i < numIntervals - 1; i++)
    {
        int start = (int)(i * pointsInInterval + 0.5F);          // Начальная точка в интервале
        int end = (int)((i + 1) * pointsInInterval + 0.5F) - 1;  // Конечная точка в интервале

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

        int deltaMIN = (int)(min * stepY + 0.5F);
        int deltaMAX = (int)(max * stepY + 0.5F);

        //Painter::DrawVLine(x + i, y0 - deltaMIN, y0 - deltaMAX);
        VLine(deltaMIN - deltaMAX).Draw(x + i, y0 - deltaMIN);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawScreenArea()
{
    float relBegin = (float)SHIFT_IN_MEMORY / FPGA::NumPoints();          // Относительное начало экранной области относительно всей памяти
    float relWidth = (float)(Grid::Width() + 1) / FPGA::NumPoints();      // Относительное (относительно всей шириный окна) ширина видимой области

    int begin = (int)(relBegin * Width() + 0.5F);   // Реальное начало экранной области относительно начала окна памяти
    int width = (int)(relWidth * Width() + 0.5F);   // Реальная ширина видимой области

    Region(width, Height()).Fill(X() + begin, Y(), Color::GRAY_20);

    Rectangle(width, Height()).Draw(X() + begin, Y(), Color::FILL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Display::MemoryWindow::Width()
{
    return 278;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Display::MemoryWindow::Height()
{
    return Grid::Top() - 2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Display::MemoryWindow::X()
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Display::MemoryWindow::Y()
{
    return 0;
}
