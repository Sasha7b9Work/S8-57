#include "defines.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Osci/Measurements/CursorsMeasurements.h"
#include "Settings/Settings.h"


static void DrawDataInRect(int x, int y, int width, int height, const uint8 *data, uint length);
/// Нарисовать область экрана
static void DrawScreenArea();


void DisplayOsci::MemoryWindow::Draw()
{
    if (CursorsMeasurements::NecessaryDraw())
    {
        return;
    }

    DrawScreenArea();

    Chan::E chans[2] = { (set.disp.lastAffectedChannel == Chan::A) ? Chan::B : Chan::A, (set.disp.lastAffectedChannel == Chan::A) ? Chan::A : Chan::B };

    for (int i = 0; i < 2; i++)
    {
        Chan::E chan = chans[i];
        if (set.ch[chan].enabled)
        {
            Color::CHAN[chan].SetAsCurrent();
            DrawDataInRect(X(), Y(), Width(), Height(), OUT(chan), ENumPointsFPGA().PointsInChannel());
        }
    }
}


static void DrawDataInRect(int x, int y, int width, int height, const uint8 *data, uint length)
{
    if (PeakDetMode().IsEnabled())
    {
        length *= 2;
    }

    int numIntervals = width + 1;                               // Количество интервалов, в которых будем рисовать наш сигнал - фактически, количество вертикальных линий
    float pointsInInterval = static_cast<float>(length) / numIntervals;      // Количество точек, рисуемых в одном интервале.

    float stepY = static_cast<float>(height) / static_cast<float>(VALUE::MAX - VALUE::MIN);

    int y0 = y + height;

    for (int i = 0; i < numIntervals - 1; i++)
    {
        int start = static_cast<int>(i * pointsInInterval + 0.5F);          // Начальная точка в интервале
        int end = static_cast<int>((i + 1) * pointsInInterval + 0.5F) - 1;  // Конечная точка в интервале

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

        int deltaMIN = static_cast<int>(min * stepY + 0.5F);
        int deltaMAX = static_cast<int>(max * stepY + 0.5F);

        VLine(deltaMIN - deltaMAX).Draw(x + i, y0 - deltaMIN);
    }
}


static void DrawScreenArea()
{
    float relBegin = static_cast<float>(SHIFT_IN_MEMORY) / ENumPointsFPGA().PointsInChannel();          // Относительное начало экранной области относительно всей памяти
    float relWidth = static_cast<float>(Grid::Width() + 1) / ENumPointsFPGA().PointsInChannel();        // Относительное (относительно всей шириный окна) ширина видимой области

    int begin = static_cast<int>(relBegin * DisplayOsci::MemoryWindow::Width() + 0.5F);                 // Реальное начало экранной области относительно начала окна памяти
    int width = static_cast<int>(relWidth * DisplayOsci::MemoryWindow::Width() + 0.5F);                 // Реальная ширина видимой области

    Region(width, DisplayOsci::MemoryWindow::Height()).Fill(DisplayOsci::MemoryWindow::X() + begin, DisplayOsci::MemoryWindow::Y(), Color::GRAY_20);

    Rectangle(width, DisplayOsci::MemoryWindow::Height()).Draw(DisplayOsci::MemoryWindow::X() + begin, DisplayOsci::MemoryWindow::Y(), Color::FILL);
}


int DisplayOsci::MemoryWindow::Width()
{
    return 270;
}


int DisplayOsci::MemoryWindow::Height()
{
    return 17;
}


int DisplayOsci::MemoryWindow::X()
{
    return 0;
}


int DisplayOsci::MemoryWindow::Y()
{
    return ::Display::HEIGHT - 19;
}
