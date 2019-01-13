#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Display_Primitives.h"
#include "Painter.h"
#include "Keyboard/DecoderDevice.h"
#include "Utils/Math.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using Display::Char;
using Display::HLine;
using Display::Point;
using Display::Rectangle;
using Display::Region;
using Display::Text;
using Display::VLine;
using Display::VPointLine;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::Draw10SymbolsInRect(int x, int y, char eChar)
{
    for (char i = 0; i < 5; i++)
    {
        //DrawChar(x + 8 * i, y, eChar + i);
        Char(eChar + i).Draw(x + 8 * i, y);

        //DrawChar(x + 8 * i, y + 8, eChar + i + 16);
        Char(eChar + i + 16).Draw(x + 8 * i, y + 8);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::Draw4SymbolsInRect(int x, int y, char eChar, Color color)
{
    Color::SetCurrent(color);
 
    for (char i = 0; i < 2; i++)
    {
        //DrawChar(x + 8 * i, y, eChar + i);
        Char(eChar + i).Draw(x + 8 * i, y);

        //DrawChar(x + 8 * i, y + 8, eChar + i + 16);
        Char(eChar + i + 16).Draw(x + 8 * i, y + 8);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLineArray(int x, int numLines, uint8 *y0y1, Color color)
{
    Color::SetCurrent(color);

    for(int i = 0; i < numLines; i++)
    {
        uint8 y0 = *y0y1++;
        uint8 y1 = *y0y1++;

        //DrawVLine(x, y0, y1);
        VLine(y1 - y0).Draw(x, y0);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawMultiVPointLine(int numLines, int y0, const uint16 *x0, int delta, int count, Color color)
{
    Color::SetCurrent(color);

    for(int i = 0; i < numLines; i++)
    {
        int x = x0[i];
        for(int numPoint = 0; numPoint < count; numPoint++)
        {
            int y = y0 + numPoint * delta;
            //SetPoint(x, y);
            Point().Draw(x, y);
        }
    }
}
