#include "defines.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Tester/Tester.h"


/// � ����� ������� �������� ��������� ����� ������-����������
typedef uint8 array8[Tester::NUM_STEPS][TESTER_NUM_POINTS];
typedef uint16 array16[Tester::NUM_STEPS][TESTER_NUM_POINTS];

static bool ready[Tester::NUM_STEPS] = {false, false, false, false, false};

static array8 *datY = (array8 *)OUT_A;
static array16 *datX = (array16 *)OUT_B;


/// �������� ������� �����������
static void DrawLegend(int x, int y);
/// ���������� ��������� ������ ������
static void DrawParametersChannel(Chan::E ch, int x, int y);
/// ���������� ����, ������� ����� �������� ��������������� "���������"
static Color ColorForStep(int step);
/// �������� ������ ��������� numStep
static void DrawData(int step);
/// ���������� �������� �������� �������� ��������������� "���������"
static String ValueForStep(int step);



void DisplayTester::Update()
{
    Painter::BeginScene(Color::BACK);

    Grid::Draw();

    for (int i = 0; i < Tester::NUM_STEPS; i++)
    {
        DrawData(i);
    }

    DrawLegend(274, 2);

    DrawParametersChannel(Chan::A, 3, 3);
    DrawParametersChannel(Chan::B, 240, 206);

    Rectangle(::Display::WIDTH - 1, ::Display::HEIGHT - 1).Draw(0, 0, Color::FILL);
    
    Menu::Draw();
}


static Color ColorForStep(int _step)
{
    static const Color colors[Tester::NUM_STEPS] = {Color::FILL, Color::GRID, Color::RED, Color::GREEN, Color::BLUE};

    if (_step < Tester::NUM_STEPS)
    {
        return colors[_step];
    }
    else
    {
        LOG_ERROR("������������ ���");
    }

    return Color::FILL;
}


static void DrawData(int numStep)
{
    if(!ready[numStep])
    {
        return;
    }

    uint16 *x = &(*datX)[numStep][0];
    uint8 *y = &(*datY)[numStep][0];
    
    uint8 mode = BUILD_MODE(set.test.viewMode, numStep, set.test.ave);

    Painter::DrawTesterData(mode, ColorForStep(numStep), x, y);
}


void DisplayTester::SetPoints(int numStep, const uint16 dx[TESTER_NUM_POINTS], const uint8 dy[TESTER_NUM_POINTS])
{
    ready[numStep] = true;

    uint16 *x = &(*datX)[numStep][0];
    uint8 *y = &(*datY)[numStep][0];

    for(int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        x[i] = dx[i];
        y[i] = dy[i];
    }
}


static void DrawLegend(int x, int y)
{
    Region(43, 40).Fill(x, y, Color::BACK);

    HLine line(9);

    for (int i = 0; i < Tester::NUM_STEPS; i++)
    {
        line.Draw(x + 1, y + 4 + i * 8, ColorForStep(i));

        ValueForStep(i).Draw(x + 12, y + i * 8);
    }
}


static String ValueForStep(int step)
{
    static pString valuesU[2][5] =
    {
        {"0 �", "0.6 �", "1.2 �", "1.8 �", "2.4 �"},
        {"0 �", "3 �",   "6 �",   "9 �",   "12 �"}
    };

    if (set.test.control == TesterControl::Voltage)
    {
        return String(valuesU[set.test.stepU][step]);
    }

    static pString valuesI[2][5] =
    {
        {"0 ���", "4 ���",  "8 ���",  "12 ���", "16 ���"},
        {"0 ���", "20 ���", "40 ���", "60 ���", "80 ���"}
    };

    return String(valuesI[set.test.stepI][step]);
}


static void DrawParametersChannel(Chan::E ch, int x, int y)
{
    Range range = Range(set.ch[ch].range);
    uint16 rShift = SET_RSHIFT(ch);
    Tester::Scale scale(range.value, ch);
    Tester::Shift shift(rShift, ch);

    Color::FILL.SetAsCurrent();
    Text(scale.ToString()).DrawOnBackground(x, y, Color::BACK);
    Color::FILL.SetAsCurrent();

    Text(shift.ToString(scale.value).CString()).DrawOnBackground(x + ((ch == Chan::A) ? 25 : 35), y, Color::BACK);
}