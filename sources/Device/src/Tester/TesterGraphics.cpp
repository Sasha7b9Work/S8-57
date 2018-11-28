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
typedef uint8 array[Chan::Number][Tester::NUM_STEPS][Tester::NUM_POINTS];

static bool ready[Tester::NUM_STEPS] = {false, false, false, false, false};

static array *dat = (array *)OUT_A;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Написать параметры вывода
static void DrawParameters(int x, int y);
/// Написать легенду изображения
static void DrawInfo(int x, int y);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tester::Graphics::Update()
{
    Painter::BeginScene(Color::BACK);

    Grid::Draw();

    int size = 239;

    for (int i = 0; i < NUM_STEPS; i++)
    {
        DrawData(i, 0, 0);
    }

    DrawParameters(size + 10, 10);

    DrawInfo(size + 10, 50);
    
    Menu::Draw();
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
void Tester::Graphics::SetPoints(int numStep, uint8 dx[NUM_POINTS], uint8 dy[NUM_POINTS])
{
    ready[numStep] = true;

    uint8 *x = &(*dat)[Chan::A][numStep][0];
    uint8 *y = &(*dat)[Chan::B][numStep][0];

    for(int i = 0; i < NUM_POINTS; i++)
    {
        int X = NUM_POINTS - (dx[i] - MIN_VALUE);
        int Y = dy[i] - MIN_VALUE;

        LIMITATION(X, 0, NUM_POINTS - 1);
        LIMITATION(Y, 0, NUM_POINTS - 1);

        x[i] = (uint8)X;
        y[i] = (uint8)Y;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawParameters(int x, int y)
{
    Painter::SetColor(Color::FILL);

    for(int8 ch = 0; ch < 2; ch++)
    {
        Chan::E chan = (Chan::E)ch;

        Painter::DrawText(x, y + ch * 10, Range(SET_RANGE(chan)).ToString(SET_DIVIDER(ch)));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DrawInfo(int x, int y)
{

}
