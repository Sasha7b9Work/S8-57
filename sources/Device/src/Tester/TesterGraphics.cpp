#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Tester.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"
#include "Data/Reader.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef uint8 array[Chan::Number][Tester::NUM_STEPS][TESTER_NUM_POINTS];

static bool ready[Tester::NUM_STEPS] = {false, false, false, false, false};

static array *dat = (array *)OUT_A;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Написать легенду изображения
static void DrawLegend(int x, int y);
/// Отображает параметры одного канала
static void DrawParametersChannel(Chan::E ch, int x, int y);
/// Возвращает цвет, которым нужно рисовать соответствующую "ступеньку"
static Color ColorForStep(int step);
/// Рисовать данные ступеньки numStep
static void DrawData(int step, int x0, int y0);
/// Возвращает числовое значение величины соответствующей "ступеньки"
static pString ValueForStep(int step);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tester::Graphics::Update()
{
    Painter::BeginScene(Color::BACK);

    Grid::Draw();

    for (int i = 0; i < NUM_STEPS; i++)
    {
        DrawData(i, 0, 0);
    }

    DrawLegend(274, 2);

    DrawParametersChannel(Chan::A, 250, 206);
    DrawParametersChannel(Chan::B, 3, 3);
    
    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RecountPoints(uint8 *points, float scale)
{
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        *points = (uint8)(*points * scale);
        points++;
    }
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
static void DrawData(int numStep, int /*x0*/, int /*y0*/)
{
    if(!ready[numStep])
    {
        //return;
    }

    uint8 *x = &(*dat)[Chan::A][numStep][0];
    uint8 *y = &(*dat)[Chan::B][numStep][0];

    RecountPoints(x, 320.0f / 256.0f);
    RecountPoints(y, 240.0f / 256.0f);
      
    if(TESTER_VIEW_MODE_IS_LINES)
    {
        Painter::DrawTesterData((uint8)TESTER_VIEW_MODE, ColorForStep(numStep), x, y);
    }
    else
    {
        Painter::SetColor(ColorForStep(numStep));
        for(int i = 1; i < 240; i++)
        {
            Painter::SetPoint(x[i], y[i]);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::Graphics::SetPoints(int numStep, uint8 dx[TESTER_NUM_POINTS], uint8 dy[TESTER_NUM_POINTS])
{
    ready[numStep] = true;

    uint8 *x = &(*dat)[Chan::A][numStep][0];
    uint8 *y = &(*dat)[Chan::B][numStep][0];

    for(int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        int X = TESTER_NUM_POINTS - (dx[i] - MIN_VALUE);
        int Y = dy[i] - MIN_VALUE;

        LIMITATION(X, 0, TESTER_NUM_POINTS - 1);
        LIMITATION(Y, 0, TESTER_NUM_POINTS - 1);

        x[i] = (uint8)X;
        y[i] = (uint8)Y;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawLegend(int x, int y)
{
    Painter::FillRegion(x, y, 43, 40, Color::BACK);

    for (int i = 0; i < Tester::NUM_STEPS; i++)
    {
        Painter::DrawHLine(y + 4 + i * 8, x + 1, x + 10, ColorForStep(i));
        Painter::DrawText(x + 12, y + i * 8, ValueForStep(i), Color::FILL);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static pString ValueForStep(int /*step*/)
{
    return "100мкА";
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawParametersChannel(Chan::E ch, int x, int y)
{
    Range range = Range(SET_RANGE(ch));
    uint16 rShift = SET_RSHIFT(ch);
    Tester::Scale scale(range.value, ch);
    Tester::Shift shift(rShift, ch);

    Painter::SetColor(Color::FILL);
    Painter::DrawTextOnBackground(x, y, scale.ToString(), Color::BACK);
    Painter::SetColor(Color::FILL);
    char buffer[50];
    Painter::DrawTextOnBackground(x + ((ch == Chan::A) ? 25 : 35), y, shift.ToString(scale.value, buffer), Color::BACK);
}
