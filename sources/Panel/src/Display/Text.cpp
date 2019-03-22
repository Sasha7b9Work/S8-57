#include "defines.h"
#include "Text.h"
#include "Painter.h"
#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Text::DrawChar(int eX, int eY, uint8 symbol, Color color)
{
    Painter::SetColor(color);

    uint8 width = Font::GetWidth(symbol);
    uint8 height = Font::GetHeight();

    for (int b = 0; b < height; b++)
    {
        if (Font::RowNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (Font::BitIsExist(symbol, b, bit))
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
