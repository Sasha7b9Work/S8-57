#include "defines.h"
#include "Data/Reader.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


using namespace Display::Primitives;
using namespace FPGA;
using namespace Osci::Settings;
using namespace Tester::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// В таком массиве хранятся считанные точки тестер-компонента
typedef uint8 array8[Tester::NUM_STEPS][TESTER_NUM_POINTS];
typedef uint16 array16[Tester::NUM_STEPS][TESTER_NUM_POINTS];

static bool ready[Tester::NUM_STEPS] = {false, false, false, false, false};

static array8 *datY = (array8 *)OUT_A;
static array16 *datX = (array16 *)OUT_B;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Написать легенду изображения
static void DrawLegend(int x, int y);
/// Отображает параметры одного канала
static void DrawParametersChannel(Chan::E ch, int x, int y);
/// Возвращает цвет, которым нужно рисовать соответствующую "ступеньку"
static Color ColorForStep(int step);
/// Рисовать данные ступеньки numStep
static void DrawData(int step);
/// Возвращает числовое значение величины соответствующей "ступеньки"
static String ValueForStep(int step);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tester::Display::Update()
{
    Painter::BeginScene(Color::BACK);

    Grid::Draw();

    for (int i = 0; i < NUM_STEPS; i++)
    {
        DrawData(i);
    }

    DrawLegend(274, 2);

    DrawParametersChannel(Chan::A, 250, 206);
    DrawParametersChannel(Chan::B, 3, 3);

    Rectangle(::Display::WIDTH - 1, ::Display::HEIGHT - 1).Draw(0, 0, Color::FILL);
    
    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static Color ColorForStep(int _step)
{
    static const Color colors[Tester::NUM_STEPS] = {Color::FILL, Color::GRID, Color::RED, Color::GREEN, Color::BLUE};

    if (_step < Tester::NUM_STEPS)
    {
        return colors[_step];
    }
    else
    {
        LOG_ERROR("Неправильный шаг");
    }

    return Color::FILL;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawData(int numStep)
{
    if(!ready[numStep])
    {
        return;
    }

    uint16 *x = &(*datX)[numStep][0];
    uint8 *y = &(*datY)[numStep][0];
    
    uint8 mode = BUILD_MODE(TESTER_VIEW_MODE, numStep, TESTER_ENUM_AVERAGE);

    Painter::DrawTesterData(mode, ColorForStep(numStep), x, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::Display::SetPoints(int numStep, const uint16 dx[TESTER_NUM_POINTS], const uint8 dy[TESTER_NUM_POINTS])
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static String ValueForStep(int step)
{
    static pString valuesU[2][5] =
    {
        {"0мВ", "100мВ", "200мВ", "300мВ", "400мВ"},
        {"0мВ", "500мВ", "1В",    "1.5В",  "2.0В"}
    };

    if (TESTER_CONTROL_IS_U)
    {
        return String(valuesU[TESTER_STEP_U][step]);
    }

    static pString valuesI[2][5] =
    {
        {"0мА", "4мА",  "8мА",  "12мА", "16мА"},
        {"0мА", "20мА", "40мА", "60мА", "80мА"}
    };

    return String(valuesI[TESTER_STEP_I][step]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawParametersChannel(Chan::E ch, int x, int y)
{
    Range range = Range(SET_RANGE(ch));
    uint16 rShift = SET_RSHIFT(ch);
    Scale scale(range.value, ch);
    Shift shift(rShift, ch);

    Color::FILL.SetAsCurrent();
    Text(scale.ToString()).DrawOnBackground(x, y, Color::BACK);
    Color::FILL.SetAsCurrent();

    Text(shift.ToString(scale.value).CString()).DrawOnBackground(x + ((ch == Chan::A) ? 25 : 35), y, Color::BACK);
}
