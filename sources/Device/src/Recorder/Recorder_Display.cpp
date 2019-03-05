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
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "FPGA/FPGA_Math.h"


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// С этой точки начинается вывод
static int startPoint = -1;

static uint16 posCursor[2] = { 100, 220 };


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

    void DrawMemoryWindow();
};
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Recorder::Display::Update()
{
    Painter::BeginScene(Color::BLACK);

    Grid::Draw();

    DrawData();

    DrawSettings(289, 0);

    //DrawSizeMemory(0, 0);

    DrawMemoryWindow();

    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::DrawSettings(int x, int y)
{
    if (Menu::OpenedPage() != PageFunction::PageRecorder::pointer)
    {
        return;
    }

    Region(30, 30).DrawBounded(x, y, Color::BACK, Color::FILL);

    Text(RECORDER_SCALE_X.ToString()).Draw(x + 2, y + 2);

    Text(Osci::Settings::Range(SET_RANGE_A).ToString()).Draw(x + 2, y + 11, Color::Channel(Chan::A));

    Text(Osci::Settings::Range(SET_RANGE_B).ToString()).Draw(x + 2, y + 20, Color::Channel(Chan::B));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::DrawSizeMemory(int x, int y)
{
    if (Menu::OpenedPage() != PageFunction::PageRecorder::pointer)
    {
        return;
    }

    Region(100, 10).DrawBounded(x, y, Color::BACK, Color::FILL);

    //Text(Integer(Storage::CurrentFrame()->Size()).ToString(false)).Draw(x + 2, y + 2);

    String text("Осталось %d сек", (int)(Storage::CurrentFrame()->FreeMemory() / RECORDER_SCALE_X.BytesToSec()));

    Text(text).Draw(x + 2, y + 1);
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
static char *DeltaTime(char buffer[20])
{
    float delta = std::fabsf((float)(posCursor[0] - posCursor[1])) * RECORDER_SCALE_X.TimeForPointMS() / 1000.0F;

    std::strcpy(buffer, Time(delta).ToString(false).CString());

    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static char *TimeCursor(int numCur, char buffer[20])
{
    int numPoint = startPoint + posCursor[numCur];

    float time = (numPoint * RECORDER_SCALE_X.TimeForPointMS()) / 1000.0F;

    std::strcpy(buffer, Time(time).ToString(false).CString());

    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static char *VoltageCursor(Chan::E ch, int numCur, char buffer[20])
{
    uint numPoint = (uint)(startPoint + posCursor[numCur]);

    Recorder::Storage::Frame *frame = Recorder::Storage::CurrentFrame();

    Recorder::Storage::Point point = frame->GetPoint(numPoint, frame->NumPoints());

    uint8 value = (uint8)((point.Min(ch) + point.Max(ch)) / 2);

    float voltage = FPGA::Math::Point2Voltage(value, SET_RANGE(ch), Osci::Settings::RShift::ZERO);

    std::strcpy(buffer, Voltage(voltage).ToString(false).CString());

    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawParametersCursors()
{
    int width = 49;

    int x = 319 - width;
    int y = 10;

    int x1 = x + 9;

    int y1 = y + 1;
    int y2 = y1 + 8;
    int y3 = y2 + 8;
    int y4 = y3 + 8;
    int y5 = y4 + 8;
    int y6 = y5 + 8;
    int y7 = y6 + 8;

    char buffer[20];

    Region(width, 58).DrawBounded(x, y, Color::BACK, Color::FILL);

    Text(String("1:%s", TimeCursor(0, buffer))).Draw(x + 2, y1, Color::FILL);

    Text(VoltageCursor(Chan::A, 0, buffer)).Draw(x1, y2, Color::Channel(Chan::A));

    Text(VoltageCursor(Chan::B, 0, buffer)).Draw(x1, y3, Color::Channel(Chan::B));

    Text(String("2:%s", TimeCursor(1, buffer))).Draw(x + 2, y4, Color::FILL);

    Text(VoltageCursor(Chan::A, 1, buffer)).Draw(x1, y5, Color::Channel(Chan::A));

    Text(VoltageCursor(Chan::B, 1, buffer)).Draw(x1, y6, Color::Channel(Chan::B));

    Text(String("dT %s", DeltaTime(buffer))).Draw(x + 2, y7, Color::FILL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawCursors()
{
    if (Menu::OpenedPage() != PageFunction::PageRecorder::PageShow::pointer)
    {
        return;
    }

    DashedVLine cursor(239, 3, 1, 0);

    Color::SetCurrent(Color::FILL);

    cursor.Draw(posCursor[0], 0);
    
    cursor.Draw(posCursor[1], 0);

    DrawParametersCursors();
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

    Storage::Point point = frame->GetPoint((Recorder::IsRunning() || startPoint < 0) ? 
                                                           ((numPoints < 320) ? (0) : (numPoints - 320))
                                                            : startPoint,
                                                            numPoints);

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

    DrawCursors();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::DrawMemoryWindow()
{
    static int prevNumPoints = 0;

    if (Menu::OpenedPage() != PageFunction::PageRecorder::PageShow::pointer || Storage::CurrentFrame()->NumPoints() == 0)
    {
        return;
    }

    int numPoints = (int)Storage::CurrentFrame()->NumPoints();

    if (prevNumPoints != numPoints)
    {
        prevNumPoints = numPoints;
        startPoint = numPoints - 319;
        if (startPoint < 0)
        {
            startPoint = 0;
        }
    }

    Region(319, 5).DrawBounded(0, 3, Color::BACK, Color::FILL);

    int width = (int)(320.0F / numPoints * 320.0F + 0.5F);

    if (width > 319)
    {
        width = 319;
    }

    int x = 0;

    if (numPoints > 320)
    {
        x = (int)((float)startPoint / numPoints * 320.0F + 0.5F);
    }

    Region(width, 10).DrawBounded(x, 0, Color::BACK, Color::FILL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::MoveLeft()
{
    if (Storage::CurrentFrame()->NumPoints() < 321)
    {
        return;
    }

    startPoint -= 320;
    if (startPoint < 0)
    {
        startPoint = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::MoveRight()
{
    if (Storage::CurrentFrame()->NumPoints() < 321)
    {
        return;
    }

    startPoint += 320;
    if (startPoint > (int)(Storage::CurrentFrame()->NumPoints() - 320))
    {
        startPoint = (int)(Storage::CurrentFrame()->NumPoints() - 320);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::MoveCursorLeft()
{
    uint16 &position = posCursor[Recorder::Settings::currentCursor];

    if (position > 0)
    {
        position--;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Display::MoveCursorRight()
{
    uint16 &position = posCursor[Recorder::Settings::currentCursor];

    if (position < 319)
    {
        position++;
    }
}
