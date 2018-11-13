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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef uint8 array[Chan::Number][Tester::NUM_STEPS][TESTER_NUM_POINTS];

static bool ready[Tester::NUM_STEPS] = {false, false, false, false, false};

static array *dat = (array *)OUT_A;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tester::Graphics::Update()
{
    Painter::BeginScene(Color::BACK);

    int size = 239;

    for (int i = 0; i < NUM_STEPS; i++)
    {
        DrawData(i, 0, 0);
    }

    Painter::DrawRectangle(0, 0, size, size, Color::FILL);
    Painter::DrawRectangle(0, 0, Display::WIDTH - 1, Display::HEIGHT - 1);

    Menu::Graphics::Draw();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::Graphics::DrawData(int numStep, int x0, int y0)
{
    if(!ready[numStep])
    {
        //return;
    }
    static const Color colors[5] = {Color::FILL, Color::GRID, Color::RED, Color::GREEN, Color::BLUE};
    
    uint8 *x = &(*dat)[Chan::A][numStep][0];
    uint8 *y = &(*dat)[Chan::B][numStep][0];

    //MathOSC::Smoothing(x, TESTER_NUM_POINTS, TESTER_NUM_SMOOTH + 1);
    //MathOSC::Smoothing(x, TESTER_NUM_POINTS, TESTER_NUM_SMOOTH + 1);

    if(TESTER_VIEW_MODE_IS_LINES)
    {
        Painter::DrawTesterData(TESTER_VIEW_MODE, colors[numStep], x, y);
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
