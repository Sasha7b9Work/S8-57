#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Recorder/Recorder_Display.h"
#include "Recorder/Recorder_Settings.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Menu/Menu.h"

#include <stm32f4xx_hal.h>
#include "Recorder/Recorder_Storage.h"
#include "Hardware/Timer.h"
#endif


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Recorder
{
namespace Display
{
    /// Изобразить установленные настройки
    void DrawSettings(int x, int y);
    /// Отобразить данные
    static void DrawData();
};
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Recorder::Display::Update()
{
    Painter::BeginScene(Color::BLACK);

    Grid::Draw();

    DrawData();

    DrawSettings(269, 0);

    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::DrawSettings(int x, int y)
{
    Region(50, 30).DrawBounded(x, y, Color::BACK, Color::FILL);

    Text(RECORDER_SCALE_X.ToString()).Draw(x + 2, y + 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::DrawData()
{
    Storage::Frame frame = Storage::CurrentFrame();

    if (frame.NumPoints() == 0)
    {
        return;
    }

    Color::SetCurrent(Color::FILL);

    Text(String("%d элементов", frame.NumPoints()), 2).Draw(10, 10);

    int x = 0;

    Storage::Point point = frame.GetPoint((frame.NumPoints() < 320) ? (0) : (frame.NumPoints() - 320));

    do
    {
        if (!point.IsEmpty())
        {
            int min = point.Min();
            int max = point.Max();
            
            VLine(max - min).Draw(x, min);
        }

        point = frame.NextPoint();
        x++;
    } while (x < 320);
}
