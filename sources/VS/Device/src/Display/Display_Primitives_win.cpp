#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include <SDL.h>
#include <cstring>


extern SDL_Renderer *renderer;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int DrawChar(int x, int y, char symbol);

static int DrawBigChar(int eX, int eY, int size, char _symbol);

static bool ByteFontNotEmpty(uint eChar, int byte);

static bool BitInFontIsExist(int eChar, int numByte, int bit);
/// Нарисовать одну горизонтальную лиинию из count точек c расстоянием delta между соседнимит точками
static void DrawHPointLine(int x, int y, int count, int delta);
/// Нарисовать одну вертикальную лиинию из count точек c расстоянием delta между соседнимит точками
static void DrawVPointLine(int x, int y, int count, int delta);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Primitives::Region::Fill(int x, int y, Color color)
{
    Color::SetCurrent(color);
    SDL_Rect rect = { x, y, width + 1, height + 1 };
    SDL_RenderFillRect(renderer, &rect);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::Rectangle::Draw(int x, int y, Color color)
{
    Color::SetCurrent(color);
    SDL_Rect rect = { x, y, width + 1, height + 1 };
    SDL_RenderDrawRect(renderer, &rect);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::HLine::Draw(int x, int y, Color color)
{
    Color::SetCurrent(color);
    SDL_RenderDrawLine(renderer, x, y, x + width, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::VLine::Draw(int x, int y, Color color)
{
    Color::SetCurrent(color);
    SDL_RenderDrawLine(renderer, x, y, x, y + height);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::Point::Draw(int x, int y, Color color)
{
    Color::SetCurrent(color);
    SDL_RenderDrawPoint(renderer, x, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::Line::Draw(Color color)
{
    Color::SetCurrent(color);
    SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Primitives::Text::DrawSmall(int x, int y, Color color)
{
    Color::SetCurrent(color);

    uint numSymbols = std::strlen(text);

    for (uint i = 0; i < numSymbols; i++)
    {
        x = DrawChar(x, y, text[i]);
        ++x;
    }

    return x;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::Text::DrawBig(int x, int y, Color color)
{
    Color::SetCurrent(color);

    uint numSymbols = std::strlen(text);

    for (uint i = 0; i < numSymbols; i++)
    {
        x = DrawBigChar(x, y, sizeOfType, text[i]);
        x += sizeOfType;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int DrawChar(int eX, int eY, char _symbol)
{
    uint8 symbol = (uint8)_symbol;

    int8 width = (int8)font->symbol[(uint8)symbol].width;
    int8 height = (int8)font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty((uint)symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    SDL_RenderDrawPoint(renderer, x, y);
                }
                x++;
            }
        }
    }

    return eX + width;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool ByteFontNotEmpty(uint eChar, int byte)
{
    static const uint8 *bytes = 0;
    static uint prevChar = (uint)-1;
    if (eChar != prevChar)
    {
        prevChar = eChar;
        bytes = font->symbol[prevChar].bytes;
    }
    return bytes[byte];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool BitInFontIsExist(int eChar, int numByte, int bit)
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
static int DrawBigChar(int eX, int eY, int size, char _symbol)
{
    uint8 symbol = (uint8)_symbol;

    int8 width = (int8)font->symbol[symbol].width;
    int8 height = (int8)font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            SDL_RenderDrawPoint(renderer, x + i, y + j);
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::MultiHPointLine::Draw(int x, Color color)
{
    Color::SetCurrent(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHPointLine(int x, int y, int count, int delta)
{
    for (int i = 0; i < count; i++)
    {
        SDL_RenderDrawPoint(renderer, x, y);
        x += delta;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::MultiVPointLine::Draw(int y, Color color)
{
    Color::SetCurrent(color);

    for (int i = 0; i < numLines; i++)
    {
        DrawVPointLine(x0[i], y, count, delta);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawVPointLine(int x, int y, int count, int delta)
{
    for (int i = 0; i < count; i++)
    {
        SDL_RenderDrawPoint(renderer, x, y);
        y += delta;
    }
}
