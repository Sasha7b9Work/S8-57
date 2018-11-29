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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tester::Graphics::Update()
{
    Painter::BeginScene(Color::BACK);

    Grid::Draw();

    for (int i = 0; i < NUM_STEPS; i++)
    {
        DrawData(i, 0, 0);
    }

    DrawLegend(250, 10);

    DrawParametersChannel(Chan::A, 258, 206);
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
void Tester::Graphics::DrawData(int numStep, int /*x0*/, int /*y0*/)
{
    if(!ready[numStep])
    {
        //return;
    }
    static const Color colors[5] = {Color::FILL, Color::GRID, Color::RED, Color::GREEN, Color::BLUE};

    uint8 *x = &(*dat)[Chan::A][numStep][0];
    uint8 *y = &(*dat)[Chan::B][numStep][0];

    RecountPoints(x, 320.0f / 256.0f);
    RecountPoints(y, 240.0f / 256.0f);
      
    if(TESTER_VIEW_MODE_IS_LINES)
    {
        Painter::DrawTesterData((uint8)TESTER_VIEW_MODE, colors[numStep], x, y);
    }
    else
    {
        Painter::SetColor(colors[numStep]);
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
static void DrawLegend(int, int)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawParametersChannel(Chan::E ch, int x, int y)
{
    Range::E range = SET_RANGE(ch);
    Divider::E divider = SET_DIVIDER(ch);
    uint16 rShift = SET_RSHIFT(ch);

    Painter::SetColor(Color::FILL);
    Painter::DrawTextOnBackground(x, y, Range(range).ToString(divider), Color::BACK);
    Painter::SetColor(Color::FILL);
    char buffer[50];
    Painter::DrawTextOnBackground(x + 15, y, RShift::ToString(rShift, range, divider, buffer), Color::BACK);

}
