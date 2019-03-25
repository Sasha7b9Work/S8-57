#include "defines.h"
#include "Text.h"
#include "Painter.h"
#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Если true - текст будет моноширинным
static uint8 minWidth = 0;
/// Горизонтальное расстояние между символами при выводе текста
static uint8 spacing = 1;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Text::SetMinWidthFont(uint8 width)
{
    minWidth = width;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Text::SetSpacing(uint8 space)
{
    spacing = space;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Text::DrawChar(int eX, int eY, uint8 symbol, Color color)
{
    Painter::SetColor(color);

    uint8 width = Font::GetWidth(symbol);
    uint8 height = Font::GetHeight();

    int delta = Font::IsBig() ? 0 : (9 - height);

    for (int row = 0; row < height; row++)
    {
        if (Font::RowNotEmpty(symbol, row))
        {
            int x = eX;
            int y = eY + row + delta;
            for (int bit = 0; bit < width; bit++)
            {
                if (Font::BitIsExist(symbol, row, bit))
                {
                    Painter::SetPoint(x, y);
                }
                x++;
            }
        }
    }

    return eX + width;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Text::DrawBigText(int eX, int eY, uint8 size, pString text)
{
    uint numSymbols = std::strlen(text);

    int x = eX;

    /// \todo Здесь явно неправильно вычисляется ширина символа

    for (uint i = 0; i < numSymbols; i++)
    {
        x = DrawBigChar(x, eY, size, (uint8)text[i]);
        x += size;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Text::DrawBigChar(int eX, int eY, int size, uint8 _symbol)
{
    uint8 symbol = (uint8)_symbol;

    uint8 width = Font::GetWidth(symbol);
    uint8 height = Font::GetHeight();

    for (int b = 0; b < height; b++)
    {
        if (Font::RowNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (Font::BitIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            Painter::SetPoint(x + i, y + j);
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
int Text::Draw(int x, int y, const char *text)
{
    uint numSymbols = std::strlen(text);
    for (uint i = 0; i < numSymbols; ++i)
    {
        int prevX = x;
        x = DrawChar(x, y, (uint8)text[i]);

        if (x - prevX < minWidth)
        {
            x = prevX + minWidth;
        }

        x += spacing;
    }

    return x;
}
