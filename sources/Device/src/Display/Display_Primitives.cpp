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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::HLine::HLine(int _width) : width(_width)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::HLine::Draw(int x, int y, Color color)
{
	Painter::SetColor(color);
	int x0 = x;
	int x1 = x0 + width;
	uint8 buffer[6] = { Command::Paint_DrawHLine, (uint8)y, (uint8)x0, (uint8)(x0 >> 8), (uint8)x1, (uint8)(x1 >> 8) };
	FSMC::WriteToPanel(buffer, 6);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::VLine::VLine(int _height) : height(_height)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::VLine::Draw(int x, int y, Color color)
{
	Painter::SetColor(color);
	int y0 = y;
	int y1 = y0 + height;
	uint8 buffer[5] = { Command::Paint_DrawVLine, (uint8)x, (uint8)(x >> 8), (uint8)y0, (uint8)y1 };
	FSMC::WriteToPanel(buffer, 5);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Point::Draw(int x, int y, Color color)
{
	Painter::SetColor(color);
	uint8 buffer[4] = { Command::Paint_SetPoint, (uint8)x, (uint8)(x >> 8), (uint8)y };
	FSMC::WriteToPanel(buffer, 4);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Line::Line(int _x0, int _y0, int _x1, int _y1) : x0(_x0), x1(_x1), y0(_y0), y1(_y1)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Line::Draw(Color color)
{
	Painter::SetColor(color);
	uint8 buffer[7] = { Command::Paint_DrawLine, (uint8)x0, (uint8)(x0 >> 8), (uint8)y0, (uint8)x1, (uint8)(x1 >> 8), (uint8)y1 };
	FSMC::WriteToPanel(buffer, 7);
}
