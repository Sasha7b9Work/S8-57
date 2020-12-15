#include "defines.h"
#include "Text.h"
#include "Painter.h"
#include <cstring>



// Если true - текст будет моноширинным
static uint8 minWidth = 0;
// Горизонтальное расстояние между символами при выводе текста
static uint8 spacing = 1;



void Text::SetMinWidthFont(uint8 width)
{
    minWidth = width;
}


void Text::SetSpacing(uint8 space)
{
    spacing = space;
}


int Text::DrawChar(int eX, int eY, uint8 symbol, Color color)
{
    Painter::SetColor(color);
    
    uint8 width = PFont::GetWidth(symbol);
    uint8 height = PFont::GetHeight();

    int delta = PFont::IsBig() ? 0 : (9 - height);

    for (int row = 0; row < height; row++)
    {
        if (PFont::RowNotEmpty(symbol, row))
        {
            int x = eX;
            int y = eY + row + delta;
            for (int bit = 0; bit < width; bit++)
            {
                if (PFont::BitIsExist(symbol, row, bit))
                {
                    Painter::SetPoint(x, y);
                }
                x++;
            }
        }
    }

    return eX + width;
}


int Text::Draw(int x, int y, const char *text)
{
    uint numSymbols = std::strlen(text); //-V2513
    for (uint i = 0; i < numSymbols; ++i)
    {
        int prevX = x;
        x = DrawChar(x, y, static_cast<uint8>(text[i]));

        if (x - prevX < minWidth)
        {
            x = prevX + minWidth;
        }

        x += spacing;
    }

    return x;
}
