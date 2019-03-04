#include "defines.h"
#include "Recorder/Recorder_Display.h"
#include "Recorder/Recorder_Settings.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Menu/Menu.h"

#include "Recorder/Recorder_Storage.h"
#include "Hardware/Timer.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include "Osci/Osci_Settings.h"
#include "Utils/Values.h"
#include "Data/Heap.h"


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
    /// Отобразить размер памяти, занимаемой данными
    void DrawSizeMemory(int x, int y);
};
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Recorder::Display::Update()
{
    Painter::BeginScene(Color::BLACK);

    Grid::Draw();

    DrawData();

    DrawSettings(289, 0);

    DrawSizeMemory(0, 0);

    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::DrawSettings(int x, int y)
{
    Region(30, 30).DrawBounded(x, y, Color::BACK, Color::FILL);

    Text(RECORDER_SCALE_X.ToString()).Draw(x + 2, y + 2);

    Text(Osci::Settings::Range(SET_RANGE_A).ToString()).Draw(x + 2, y + 11, Color::Channel(Chan::A));

    Text(Osci::Settings::Range(SET_RANGE_B).ToString()).Draw(x + 2, y + 20, Color::Channel(Chan::B));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::DrawSizeMemory(int x, int y)
{
    Region(50, 20).DrawBounded(x, y, Color::BACK, Color::FILL);

    Text(Integer(Storage::CurrentFrame()->Size()).ToString(false)).Draw(x + 2, y + 2);

    uint freeMemory = Heap::Size() - Storage::CurrentFrame()->Size();

    Text(Integer((int)(freeMemory / RECORDER_SCALE_X.BytesToSec())).ToString(false)).Draw(x + 2, y + 11);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int Y(int value)
{
    int delta = FPGA::VALUE::AVE - value;

    float scale = 120.0F / 125.0F;

    int y = 120 + (int)(delta * scale);

    if (y < 0)
    {
        y = 0;
    }
    if (y > 239)
    {
        y = 239;
    }

    return y;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::DrawData()
{
    Storage::Frame *frame = Storage::CurrentFrame();

    uint numPoints = frame->NumPoints();

    if (numPoints == 0)
    {
        return;
    }

    Color::SetCurrent(Color::FILL);

    int x = 0;

    Storage::Point point = frame->GetPoint((numPoints < 320) ? (0) : (numPoints - 320), numPoints);

    do
    {
        if (!point.IsEmpty())
        {
            if (RECORDER_SOURCE_A_IS_ENABLED)
            {
                int min = Y(point.Min(Chan::A));
                int max = Y(point.Max(Chan::A));

                VLine(max - min).Draw(x, min, Color::Channel(Chan::A));
            }
            if (RECORDER_SOURCE_B_IS_ENABLED)
            {
                int min = Y(point.Min(Chan::B));
                int max = Y(point.Max(Chan::B));

                VLine(max - min).Draw(x, min, Color::Channel(Chan::B));
            }
        }

        point = frame->NextPoint(numPoints);
        x++;
    } while (x < 320);
}
