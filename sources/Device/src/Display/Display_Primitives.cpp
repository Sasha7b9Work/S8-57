#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Command.h"
#include "Display_Primitives.h"
#include "Painter.h"
#include "Hardware/FSMC.h"
#include <cstring>
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
    Color::SetCurrent(color);
    uint8 buffer[7] = { Command::Paint_FillRegion, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    FSMC::WriteToPanel(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Region::DrawBounded(int x, int y, Color colorFill, Color colorBound)
{
    Color color = Color::GetCurent();
    Rectangle(width, height).Draw(x, y, colorBound);
    Region(width - 2, height - 2).Draw(x + 1, y + 1, colorFill);
    /// \todo Почему-то цвет не восстанавливается
    Color::SetCurrent(color);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Rectangle::Rectangle(int _width, int _height) : width(_width), height(_height)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Rectangle::Draw(int x, int y, Color color)
{
    Color::SetCurrent(color);
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
	Color::SetCurrent(color);
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
	Color::SetCurrent(color);
	int y0 = y;
	int y1 = y0 + height;
	uint8 buffer[5] = { Command::Paint_DrawVLine, (uint8)x, (uint8)(x >> 8), (uint8)y0, (uint8)y1 };
	FSMC::WriteToPanel(buffer, 5);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Point::Draw(int x, int y, Color color)
{
	Color::SetCurrent(color);
	uint8 buffer[4] = { Command::Paint_SetPoint, (uint8)x, (uint8)(x >> 8), (uint8)y };
	FSMC::WriteToPanel(buffer, 4);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Line::Line(int _x0, int _y0, int _x1, int _y1) : x0(_x0), y0(_y0), x1(_x1), y1(_y1)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Line::Draw(Color color)
{
	Color::SetCurrent(color);
	uint8 buffer[7] = { Command::Paint_DrawLine, (uint8)x0, (uint8)(x0 >> 8), (uint8)y0, (uint8)x1, (uint8)(x1 >> 8), (uint8)y1 };
	FSMC::WriteToPanel(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Char::Char(char _ch) : ch(_ch)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Char::Draw(int x, int y, Color color)
{
	String("%c", ch).Draw(x, y, color);

	return x + Font::GetLengthSymbol(ch) + 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Text::Text(const char *_text, uint8 _size) : text(_text), size(_size)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Display::Text::Text(const String &string, uint8 _size) : size(_size)
{
    text = string.CString();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Text::Draw(int x, int y, Color color)
{
    if (size != 1)
    {
        DrawBig(x, y, color);
    }
    else
    {
        return DrawSmall(x, y, color);
    }

    return x;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Text::DrawBig(int x, int y, Color color)
{
#define MAX_SIZE_BUFFER 100

    Color::SetCurrent(color);

    uint numSymbols = std::strlen(text); //-V2513
    uint8 buffer[MAX_SIZE_BUFFER] = { Command::Paint_DrawBigText, (uint8)x, (uint8)(x >> 8), (uint8)y, size, (uint8)(numSymbols) };

    uint8 *pointer = &buffer[6];

    while (*text)
    {
        *pointer++ = (uint8)*text++;
    }

    FSMC::WriteToPanel(buffer, 1 + 2 + 1 + 1 + numSymbols + 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Text::DrawWithLimitation(int x, int y, int limitX, int limitY, int limitWidth, int limitHeight)
{
    int retValue = x;

    while (*text)
    {
        x = DrawCharWithLimitation(x, y, *text, limitX, limitY, limitWidth, limitHeight);
        retValue += Font::GetLengthSymbol(*text);
        text++;
    }

    return retValue + 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Text::DrawCharWithLimitation(int eX, int eY, char _symbol, int limitX, int limitY, int limitWidth, int limitHeight)
{
    uint8 symbol = (uint8)_symbol;

    int8 width = (int8)font->symbol[symbol].width;
    int8 height = (int8)font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    if ((x >= limitX) && (x <= (limitX + limitWidth)) && (y >= limitY) && (y <= limitY + limitHeight))
                    {
                        Point().Draw(x, y);
                    }
                }
                x++;
            }
        }
    }

    return eX + width + 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Display::Text::ByteFontNotEmpty(uint eChar, int byte)
{
    static const uint8 *bytes = 0;
    static uint prevChar = 0xffffffffU;
    if (eChar != prevChar)
    {
        prevChar = eChar;
        bytes = font->symbol[prevChar].bytes;
    }
    return bytes[byte];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Display::Text::BitInFontIsExist(int eChar, int numByte, int bit)
{
    static uint8 prevByte = 0;      /// \todo здесь точно статики нужны?
    static int prevChar = -1;
    static int prevNumByte = -1;
    if (prevNumByte != numByte || prevChar != eChar)
    {
        prevByte = font->symbol[eChar].bytes[numByte];
        prevChar = eChar;
        prevNumByte = numByte;
    }
    return prevByte & (1 << bit);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Text::DrawInCenterRect(int eX, int eY, int width, int eHeight, Color color)
{
    int lenght = Font::GetLengthText(text);
    int height = Font::GetHeightSymbol(text[0]);
    int x = eX + (width - lenght) / 2;
    int y = eY + (eHeight - height) / 2 + 1;
    return Draw(x, y, color);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Text::DrawSmall(int x, int y, Color color)
{
    /// \todo Такую проверку нужно сделать и на приёмной стороне и тогда здесь убрать

    if (*text == 0)
    {
        return x;
    }

    int result = x + Font::GetLengthText(text) + 1;

#define MAX_SIZE_BUFFER 100

    if (std::strlen(text) + 1 > MAX_SIZE_BUFFER) //-V2513
    {
        return x + 10;
    }

    Color::SetCurrent(color);

    uint8 buffer[MAX_SIZE_BUFFER] = { Command::Paint_DrawText, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)std::strlen(text) };

    uint8 *pointer = &buffer[5];

    while (*text)
    {
        *pointer++ = (uint8)*text++;
    }

    FSMC::WriteToPanel(buffer, 1 + 2 + 1 + 1 + std::strlen(text));

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Text::DrawOnBackground(int x, int y, Color colorBackground)
{
    int width = Font::GetLengthText(text);
    int height = Font::GetSize();

    Color colorText(Color::GetCurent());
    Region(width, height).Draw(x - 1, y, colorBackground);

    Color::SetCurrent(colorText);

    //return DrawText(x, y, text);
    return Text(text).Draw(x, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Text::DrawRelativelyRight(int xRight, int y, Color color)
{
    int lenght = Font::GetLengthText(text);
    //DrawText(xRight - lenght, y, text, color);
    Text(text).Draw(xRight - lenght, y, color);
}
