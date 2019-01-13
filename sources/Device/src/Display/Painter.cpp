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
int Painter::DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color colorBackground, Color colorFill)
{
    //FillBoundedRegion(x, y, width, height, colorBackground, colorFill);
    Region(width, height).DrawBounded(x, y, colorBackground, colorFill);

    Color::SetCurrent(colorFill);
    //return DrawStringInCenterRect(x, y, width, height, text);

    return Text(text).DrawInCenterRect(x, y, width, height);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void Painter::FillBoundedRegion(int x, int y, int widht, int height, Color colorFill, Color colorBound)
{
    Region(widht - 2, height - 2).Draw(x + 1, y + 1, colorFill);
    Rectangle(widht, height).Draw(x, y, colorBound);
}
*/

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
void Painter::DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color colorText, int widthBorder, Color colorBackground)
{
    int lenght = Font::GetLengthText(text);
    //int eX = DrawStringInCenterRect(x, y, width, height, text, colorBackground);
    int eX = Text(text).DrawInCenterRect(x, y, width, height, colorBackground);
    int w = lenght + widthBorder * 2 - 2;
    int h = 7 + widthBorder * 2 - 1;
    Region(w, h).Draw(eX - lenght - widthBorder, y - widthBorder + 1);
    //DrawStringInCenterRect(x, y, width, height, text, colorText);
    Text(text).DrawInCenterRect(x, y, width, height, colorText);
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawMultiHPointLine(int numLines, int x0, const uint8 *y0, int delta, int count, Color color)
{
    Color::SetCurrent(color);

    for(int i = 0; i < numLines; i++)
    {
        int y = y0[i];
        for(int numPoint = 0; numPoint < count; numPoint++)
        {
            int x = x0 + numPoint * delta;
            //SetPoint(x, y);
            Point().Draw(x, y);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawHPointLine(int y, int x0, int x1, float delta)
{
    for (int x = x0; x <= x1; x += (int)delta)
    {
        //SetPoint(x, y);
        Point().Draw(x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVPointLine(int x, int y0, int y1, float delta)
{
    for (int y = y0; y <= y1; y += (int)delta)
    {
        //SetPoint(x, y);
        Point().Draw(x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawDashedVLine(int x, int y0, int y1, int deltaFill, int deltaEmtpy, int deltaStart) //-V2506
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmtpy))
    {
        LOG_ERROR("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int y = y0;
    if (deltaStart != 0)                 // Если линию нужно рисовать не с начала штриха
    {
        y += (deltaFill + deltaEmtpy - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            //DrawVLine(x, y0, y - 1);
            VLine(y - y0 - 1).Draw(x, y0);
        }
    }

    while (y < y1)
    {
        //DrawVLine(x, y, y + deltaFill - 1);
        VLine(deltaFill - 1).Draw(x, y);

        y += (deltaFill + deltaEmtpy);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawDashedHLine(int y, int x0, int x1, int deltaFill, int deltaEmpty, int deltaStart) //-V2506
{
    if (deltaStart < 0 || deltaStart >= (deltaFill + deltaEmpty))
    {
        LOG_ERROR("Неправильный аргумент deltaStart = %d", deltaStart);
        return;
    }
    int x = x0;
    if (deltaStart != 0)                // Если линию нужно рисовать не с начала штриха
    {
        x += (deltaFill + deltaEmpty - deltaStart);
        if (deltaStart < deltaFill)     // Если начало линии приходится на штрих
        {
            //DrawHLine(y, x0, x - 1);
			HLine(x - 1 - x0).Draw(x0, y);
        }
    }

    while (x < x1)
    {
        //DrawHLine(y, x, x + deltaFill - 1);
		HLine(deltaFill - 1).Draw(x, y);

        x += (deltaFill + deltaEmpty);
    }
}
