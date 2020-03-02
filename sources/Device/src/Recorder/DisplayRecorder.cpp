#include "defines.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Display/Warnings.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Recorder/DisplayRecorder.h"
#include "Recorder/StorageRecorder.h"
#include "Settings/Settings.h"


/// С этой точки начинается вывод
static int startPoint = -1;

//static uint16 posCursor[2] = { 100, 220 };


/// Изобразить установленные настройки
static void DrawSettings(int x, int y);
/// Отобразить данные
static void DrawData();

static void DrawMemoryWindow();



void DisplayRecorder::Update()
{
    Painter::BeginScene(Color::BLACK);

    Grid::Draw();

    DrawData();

    DrawSettings(289, 0);

    //DrawSizeMemory(0, 0);

    DrawMemoryWindow();

    DFont::Set(DTypeFont::_8);

    Warnings::Draw();

    Menu::Draw();
}


static void DrawSettings(int x, int y)
{
    if (Menu::OpenedItem() != PageRecorder::self)
    {
        return;
    }

    Region(30, 30).DrawBounded(x, y, Color::BACK, Color::FILL);

    Text(Recorder::ScaleX::ToString()).Draw(x + 2, y + 2);

    Text(Range(Chan::A).ToString(static_cast<int8>(Divider(Chan::A)))).Draw(x + 2, y + 11, Color::CHAN[Chan::A]);

    Text(Range(Chan::B).ToString(static_cast<int8>(Divider(Chan::B)))).Draw(x + 2, y + 20, Color::CHAN[Chan::B]);
}


//static int Y(int value)
//{
//    int delta = VALUE::AVE - value;
//
//    float scale = 120.0F / 125.0F;
//
//    int y = 120 + static_cast<int>(delta * scale);
//
//    if (y < 0)
//    {
//        y = 0;
//    }
//    if (y > 239)
//    {
//        y = 239;
//    }
//
//    return y;
//}


//static char *DeltaTime(char buffer[20])
//{
//    float delta = std::fabsf(static_cast<float>(posCursor[0] - posCursor[1])) * Recorder::ScaleX::TimeForPointMS() / 1000.0F;
//
//    std::strcpy(buffer, Time(delta).ToString(false).c_str());
//
//    return buffer;
//}


//static char *TimeCursor(int numCur, char buffer[20])
//{
//    int numPoint = startPoint + posCursor[numCur];
//
//    float time = (static_cast<float>(numPoint) * Recorder::ScaleX::TimeForPointMS()) / 1000.0F;
//
//    std::strcpy(buffer, Time(time).ToString(false).c_str());
//
//    return buffer;
//}


//static char *VoltageCursor(Chan::E , int , char [20])
//{
//    uint numPoint = static_cast<uint>(startPoint + posCursor[numCur]);
//
//    Record *frame = StorageRecorder::CurrentRecord();
//
//    Point point = frame->GetPoint(numPoint, frame->NumPoints());
//
//    uint8 value = static_cast<uint8>((point.Min(ch) + point.Max(ch)) / 2);
//
//    float voltage = VALUE::ToVoltage(value, Range(ch), 0);
//
//    std::strcpy(buffer, Voltage(voltage).ToString(false).c_str());
//
//    return buffer;
//}


//static void DrawParametersCursors()
//{
//    int width = 49;
//
//    int x = 319 - width;
//    int y = 10;
//
//    int x1 = x + 9;
//
//    int y1 = y + 1;
//    int y2 = y1 + 8;
//    int y3 = y2 + 8;
//    int y4 = y3 + 8;
//    int y5 = y4 + 8;
//    int y6 = y5 + 8;
//    int y7 = y6 + 8;
//
//    char buffer[20];
//
//    Region(width, 58).DrawBounded(x, y, Color::BACK, Color::FILL);
//
//    Text(String("1:%s", TimeCursor(0, buffer))).Draw(x + 2, y1, Color::FILL);
//
//    Text(VoltageCursor(Chan::A, 0, buffer)).Draw(x1, y2, Color::CHAN[Chan::A]);
//
//    Text(VoltageCursor(Chan::B, 0, buffer)).Draw(x1, y3, Color::CHAN[Chan::B]);
//
//    Text(String("2:%s", TimeCursor(1, buffer))).Draw(x + 2, y4, Color::FILL);
//
//    Text(VoltageCursor(Chan::A, 1, buffer)).Draw(x1, y5, Color::CHAN[Chan::A]);
//
//    Text(VoltageCursor(Chan::B, 1, buffer)).Draw(x1, y6, Color::CHAN[Chan::B]);
//
//    Text(String("dT %s", DeltaTime(buffer))).Draw(x + 2, y7, Color::FILL);
//}


//static void DrawCursors()
//{
//    if (Menu::OpenedItem() != PageRecorder::Show::self)
//    {
//        return;
//    }
//
//    DashedVLine cursor(239, 3, 1, 0);
//
//    Color::FILL.SetAsCurrent();
//
//    cursor.Draw(posCursor[0], 0);
//    
//    cursor.Draw(posCursor[1], 0);
//
//    DrawParametersCursors();
//}


static void DrawData()
{
//    Record *record = StorageRecorder::CurrentRecord();
//
//    uint numPoints = record->NumPoints();
//
//    if (numPoints == 0)
//    {
//        return;
//    }
//
//    Color::FILL.SetAsCurrent();
//
//    int x = 0;
//
//    Point point = record->GetPoint((Recorder::IsRunning() || startPoint < 0) ? 
//                                                           ((numPoints < 320) ? (0) : (numPoints - 320))
//                                                            : startPoint,
//                                                            numPoints);
//
//    do
//    {
//        if (!point.IsEmpty())
//        {
//            if (Recorder::IsEnabledA())
//            {
//                int min = Y(point.Min(Chan::A));
//                int max = Y(point.Max(Chan::A));
//
//                VLine(max - min).Draw(x, min, Color::CHAN[Chan::A]);
//            }
//            if (Recorder::IsEnabledB())
//            {
//                int min = Y(point.Min(Chan::B));
//                int max = Y(point.Max(Chan::B));
//
//                VLine(max - min).Draw(x, min, Color::CHAN[Chan::B]);
//            }
//        }
//
//        point = record->NextPoint(numPoints);
//        x++;
//    } while (x < 320);
//
//    DrawCursors();
}


static void DrawMemoryWindow()
{
    static int prevNumPoints = 0;

    if (Menu::OpenedItem() != PageRecorder::Show::self || StorageRecorder::LastRecord()->NumPoints() == 0)
    {
        return;
    }

    int numPoints = static_cast<int>(StorageRecorder::LastRecord()->NumPoints());

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

    int width = static_cast<int>(320.0F / numPoints * 320.0F + 0.5F);

    if (width > 319)
    {
        width = 319;
    }

    int x = 0;

    if (numPoints > 320)
    {
        x = static_cast<int>(static_cast<float>(startPoint) / numPoints * 320.0F + 0.5F);
    }

    Region(width, 10).DrawBounded(x, 0, Color::BACK, Color::FILL);
}


void DisplayRecorder::MoveLeft()
{
    if (StorageRecorder::LastRecord()->NumPoints() < 321)
    {
        return;
    }

    startPoint -= 320;
    if (startPoint < 0)
    {
        startPoint = 0;
    }
}


void DisplayRecorder::MoveRight()
{
    if (StorageRecorder::LastRecord()->NumPoints() < 321)
    {
        return;
    }

    startPoint += 320;
    if (startPoint > static_cast<int>(StorageRecorder::LastRecord()->NumPoints() - 320))
    {
        startPoint = static_cast<int>(StorageRecorder::LastRecord()->NumPoints() - 320);
    }
}


void DisplayRecorder::MoveCursorLeft()
{
}


void DisplayRecorder::MoveCursorRight()
{
}
