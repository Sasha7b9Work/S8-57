#include "Painter.h"
#include <cstring>
#include "Display/Font/Font.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Painter::DrawText(int x, int y, const char *text, Color color)
{
    SetColor(color);
    
    uint numSymbols = std::strlen(text);
    for (uint i = 0; i < numSymbols; ++i)
    {
        x = DrawChar(x, y, text[i]);
        ++x;
    }

    return x;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawBigText(int eX, int eY, uint8 size, pString text)
{
    uint numSymbols = std::strlen(text);

    int x = eX;

    /// \todo Здесь явно неправильно вычисляется ширина символа

    for (uint i = 0; i < numSymbols; i++)
    {
        x = DrawBigChar(x, eY, size, text[i]);
        x += size;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawBigChar(int eX, int eY, int size, char _symbol)
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
                            SetPoint(x + i, y + j);
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
void Painter::SetFont(Font::Type typeFont)
{
    font = fonts[typeFont];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawChar(int eX, int eY, char symbol, Color color)
{
    SetColor(color);

    int8 width = (int8)font->symbol[symbol].width;
    int8 height = (int8)font->height;

    int size = 1;

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
                            SetPoint(x + i, y + j);
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}
