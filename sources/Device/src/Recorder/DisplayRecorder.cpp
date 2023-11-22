// 2023/11/22 13:46:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Display/Warnings.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Recorder/DisplayRecorder.h"
#include "Recorder/StorageRecorder.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"
#include <cmath>
#include <cstring>


namespace DisplayRecorder
{
    static Record *displayed = nullptr;             // Текущая отображаемая запись
    static int startPoint = -1;                     // С этой точки начинается вывод
    static int posCursor[2] = { 100, 220 };
    static bool inProcessUpdate = false;            // true, если в данный момент происходит отрисовка


    DisplayRecorder::SpeedWindow DisplayRecorder::speed = DisplayRecorder::SpeedWindow::_1Window;


    // Изобразить установленные настройки
    static void DrawSettings(int x, int y);

    // Отобразить данные
    static void DrawData();

    // Нарисовать данные канала, начиная с точки start
    static void DrawChannel(Chan::E ch);

    static void DrawMemoryWindow();

    static void DrawCursors();

    static void DrawParametersCursors();

    static char *TimeCursor(int numCur, char buffer[30]);

    static char *VoltageCursor(Chan::E, int, char[30]);

    // Дописывает в buffer символьное представление value
    static void VoltagePoint(Chan::E ch, uint8 value, char buffer[30]);

    static char *DeltaTime(char buffer[30]);

    // Возвращает указатель на переменную с позицией текущего курсора (0, если курсор для перемещения не выбран)
    static int *CurrentPosCursor();

    // Значок, который показывает, в каком состоянии сейчас находится регистратор
    struct RecordIcon
    {
        static void Upate(int x, int y);
    };
}


void DisplayRecorder::Update()
{
    Painter::BeginScene(Color::BLACK);

    Grid::Draw();

    inProcessUpdate = true;

    DrawData();

    inProcessUpdate = false;

    StorageRecorder::LastRecord()->AddMissingPoints();

    DrawSettings(289, 0);

    DrawCursors();

    DrawMemoryWindow();

    DFont::Set(DTypeFont::_8);

    Warnings::Draw();

    Menu::Draw();

    RecordIcon::Upate(5, 5);
}


void DisplayRecorder::DrawSettings(int x, int y)
{
    if (Menu::OpenedItem() == PageRecorder::self)
    {
        Region(30, 30).DrawBounded(x, y, Color::BACK, Color::FILL);

        Text(Recorder::ScaleX::ToString()).Draw(x + 2, y + 2);

        Text(Range::ToString(ChanA, S_DIVIDER_A)).Draw(x + 2, y + 11, Color::CHAN[ChanA]);

        Text(Range::ToString(ChanB, S_DIVIDER_B)).Draw(x + 2, y + 20, Color::CHAN[ChanB]);
    }
}


static int Y(int value)
{
    int delta = VALUE::AVE - value;

    float scale = 120.0F / 125.0F;

    int y = 120 + static_cast<int>(delta * scale);

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


char *DisplayRecorder::DeltaTime(char buffer[30])
{
    float delta = std::fabsf(static_cast<float>(posCursor[0] - posCursor[1])) * Recorder::ScaleX::TimeForPointMS() / 1000.0F;

    std::strcpy(buffer, Time(delta).ToString(false).c_str()); //-V2513

    return buffer;
}


char *DisplayRecorder::TimeCursor(int numCur, char buffer[30])
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    int numPoint = startPoint + posCursor[numCur];

    if (numPoint < displayed->NumPoints())
    {
        PackedTime time = displayed->timeStart;

        time.AddTime((startPoint + posCursor[numCur]) * displayed->timeForPointMS);

        std::strcpy(buffer, time.ToString().c_str()); //-V2513
    }
    else
    {
        std::strcpy(buffer, "..."); //-V2513
    }

    return buffer;
}


void DisplayRecorder::VoltagePoint(Chan::E ch, uint8 value, char buffer[30])
{
    if (value > VALUE::MAX)
    {
        std::strcat(buffer, "\x9d"); //-V2513
    }
    else if (value < VALUE::MIN)
    {
        std::strcat(buffer, "\xb9"); //-V2513
    }
    else
    {
        float voltage = VALUE::ToVoltage(value, S_RANGE(ch), 0);

        std::strcat(buffer, Voltage(voltage).ToString(true).c_str()); //-V2513
    }
}


char *DisplayRecorder::VoltageCursor(Chan::E ch, int numCur, char buffer[30])
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    int numPoint = startPoint + posCursor[numCur];

    Point16 *point = (ch == Chan::A) ? displayed->ValueA(numPoint) : displayed->ValueB(numPoint);

    if (numPoint < displayed->NumPoints())
    {
        buffer[0] = '\0';
        VoltagePoint(ch, point->min, buffer);
        std::strcat(buffer, " : "); //-V2513
        VoltagePoint(ch, point->max, buffer);
    }
    else
    {
        std::strcpy(buffer, "..."); //-V2513
    }

    return buffer;
}


static char *VoltageSensor(int, char[30])
{
    return "";
}


void DisplayRecorder::DrawParametersCursors()
{
    if (!S_REC_INFO_IS_SHOWN)
    {
        return;
    }

#define DRAW_IF_ENABLED(en, text) if(en) { text; y +=d; }

    HAL_BUS_CONFIGURE_TO_FSMC();

    bool enA = displayed->ContainsChannelA();
    bool enB = displayed->ContainsChannelB();
    bool enSensor = displayed->ContainsSensor();

    int width = 100;
    int height = 29;
    int x = 319 - width;
    int y = 11;
    int d = 9;
    int x1 = x + 9;

    int dHeight = 2 * 9;

    if (enA)      { height += dHeight; }
    if (enB)      { height += dHeight; }
    if (enSensor) { height += dHeight; }

    Region(width + 2, height).DrawBounded(x - 2, 10, Color::BACK, Color::FILL);

    char buffer[30];
    
    Text(String("1:%s", TimeCursor(0, buffer))).Draw(x, y, Color::FILL);

    y += d;

    DRAW_IF_ENABLED(enA, Text(VoltageCursor(ChanA, 0, buffer)).Draw(x1, y, Color::CHAN[ChanA]));

    DRAW_IF_ENABLED(enB, Text(VoltageCursor(ChanB, 0, buffer)).Draw(x1, y, Color::CHAN[ChanB]));

    DRAW_IF_ENABLED(enSensor, Text(VoltageSensor(0, buffer)).Draw(x1, y, Color::FILL));

    Text(String("2:%s", TimeCursor(1, buffer))).Draw(x, y, Color::FILL);

    y += d;

    DRAW_IF_ENABLED(enA, Text(VoltageCursor(ChanA, 1, buffer)).Draw(x1, y, Color::CHAN[ChanA]));

    DRAW_IF_ENABLED(enB, Text(VoltageCursor(ChanB, 1, buffer)).Draw(x1, y, Color::CHAN[ChanB]));

    DRAW_IF_ENABLED(enSensor, Text(VoltageSensor(1, buffer)).Draw(x1, y, Color::FILL));

    Text(String("dT %s", DeltaTime(buffer))).Draw(x, y, Color::FILL);
}


void DisplayRecorder::DrawCursors()
{
    if (Menu::OpenedItem() == PageRecorder::Show::self || Menu::OpenedItem()->Keeper() == PageRecorder::Show::self)
    {
        DashedVLine cursor(239, 3, 1, 0);

        Color::FILL.SetAsCurrent();

        cursor.Draw(posCursor[0], 0);

        cursor.Draw(posCursor[1], 0);

        DrawParametersCursors();
    }
}


void DisplayRecorder::DrawData()
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    if(displayed->ContainsChannelA())
    {
        DrawChannel(ChanA);
    }

    if(displayed->ContainsChannelB())
    {
        DrawChannel(ChanB);
    }
}


void DisplayRecorder::DrawChannel(Chan::E ch)
{
    int numPoints = displayed->NumPoints();

    if(numPoints == 0)
    {
        return;
    }

    typedef Point16 *(Record::*funcValue)(int);

    funcValue funcs[2] = { &Record::ValueA, &Record::ValueB };

    funcValue func = funcs[ch];

    int index = (numPoints < 320) ? 0 : (numPoints - 320);

    if (startPoint >= 0)
    {
        index = startPoint;
    }

    Point16 *point = (displayed->*func)(index);

    for(int x = 0; x < 320; x++)
    {
        if(!point->IsEmpty())
        {
            int min = Y(point->min);
            int max = Y(point->max);

            VLine(max - min).Draw(x, min, Color::CHAN[ch]);
        }

        HAL_BUS_CONFIGURE_TO_FSMC();

        point = point->Next(displayed);
    };
}


void DisplayRecorder::DrawMemoryWindow()
{
    static int prevNumPoints = 0;

    HAL_BUS_CONFIGURE_TO_FSMC();

    if ((Menu::OpenedItem() != PageRecorder::Show::self && Menu::OpenedItem()->Keeper() != PageRecorder::Show::self) || displayed->NumPoints() == 0)
    {
        return;
    }

    int numPoints = static_cast<int>(displayed->NumPoints());

    if (prevNumPoints != numPoints)
    {
        prevNumPoints = numPoints;
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


void DisplayRecorder::MoveWindowLeft()
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    if (displayed->NumPoints() < 321)
    {
        return;
    }

    startPoint -= speed.NumPoints();

    if (startPoint < 0)
    {
        startPoint = 0;
    }
}


void DisplayRecorder::MoveWindowRight()
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    if (displayed->NumPoints() < 321)
    {
        return;
    }

    startPoint += speed.NumPoints();

    if (startPoint > static_cast<int>(displayed->NumPoints() - 320))
    {
        startPoint = static_cast<int>(displayed->NumPoints() - 320);
    }
}


void DisplayRecorder::MoveCursorLeft()
{
    Math::LimitationDecrease(CurrentPosCursor(), 0);
}


void DisplayRecorder::MoveCursorRight()
{
    Math::LimitationIncrease(CurrentPosCursor(), 319);
}


int *DisplayRecorder::CurrentPosCursor()
{
    static int nullPos;
    int *result = &nullPos;

    if (S_REC_CURSOR_IS_1)      { result = &posCursor[0]; }
    else if (S_REC_CURSOR_IS_2) { result = &posCursor[1]; } //-V2516

    return result;
}


bool DisplayRecorder::InProcessUpdate()
{
    return inProcessUpdate;
}


void DisplayRecorder::RecordIcon::Upate(int x, int y)
{
    if (Menu::OpenedItem() != PageRecorder::self)
    {
        return;
    }

    if (Recorder::InRecordingMode())
    {
        static uint timeStart = 0;              // Время начала цикла (зажёгся/потух)

        while (TIME_MS - timeStart >= 1000)
        {
            timeStart += 1000;
        }

        if (TIME_MS - timeStart <= 500)
        {
            Circle(6).Fill(x, y, Color::RED);
        }
    }
    else
    {
        Region(10, 10).Fill(x, y, Color::GREEN);
    }
}


void DisplayRecorder::SetDisplayedRecord(Record *record, bool forListening)
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    displayed = record;

    if (forListening)
    {
        startPoint = -1;
    }
    else
    {
        startPoint = 0;

        if (displayed)
        {
            if (displayed->NumPoints() > 320)
            {
                startPoint = displayed->NumPoints() - 320;
            }
        }
    }
}


int DisplayRecorder::SpeedWindow::NumPoints() const
{
    static const int nums[3] = { 20, 320, 320 * 10 };

    return nums[value];
}


