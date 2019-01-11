#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Command.h"
#include "Display_Primitives.h"
#include "Painter.h"
#include "Hardware/FSMC.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Display::Primitive::Primitive(int _x, int _y) : x(_x), y(_y), color(Color::NUMBER)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Region::Region(int _width, int _height) : width(_width), height(_height)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Region::Draw(int x, int y, Color color)
{
    Painter::SetColor(color);
    uint8 buffer[7] = { Command::Paint_FillRegion, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    FSMC::WriteToPanel(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Rectangle::Rectangle(int _width, int _height) : width(_width), height(_height)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Rectangle::Draw(int x, int y, Color color)
{
    Painter::SetColor(color);
    uint8 buffer[7] = { Command::Paint_DrawRectangle, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    FSMC::WriteToPanel(buffer, 7);
}
