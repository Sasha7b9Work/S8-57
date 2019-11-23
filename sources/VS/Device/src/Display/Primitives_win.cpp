#include "defines.h"
#include "common/Transceiver.h"
#include "Display/Primitives.h"
#include "Display/Painter.h"
#include <cstring>
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


extern wxColour colorDraw;
extern wxMemoryDC memDC;


static int DrawChar(int x, int y, char symbol);

static int DrawBigChar(int eX, int eY, int size, char _symbol);
/// Нарисовать одну горизонтальную лиинию из count точек c расстоянием delta между соседнимит точками
static void DrawHPointLine(int x, int y, int count, int delta);
/// Нарисовать одну вертикальную лиинию из count точек c расстоянием delta между соседнимит точками
static void DrawVPointLine(int x, int y, int count, int delta);


void Region::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();
    memDC.GradientFillLinear({ x, y, width + 1, height + 1 }, colorDraw, colorDraw);
}


void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    //memDC.DrawRectangle({ x, y, width + 1, height + 1 });
    Transceiver::Send(nullptr, 0);                            // Это нужно лишь для того, чтобы регистратор читал точки
}


void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    memDC.DrawLine({ x, y }, { x + width, y });
}


void VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    memDC.DrawLine({ x, y }, { x, y + height });
}


void Pixel::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    memDC.DrawPoint({ x, y });
}


void Line::Draw(Color color)
{
    color.SetAsCurrent();
    memDC.DrawLine({ x0, y0 }, { x1, y1 });
}


int Text::DrawSmall(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint numSymbols = std::strlen(text);

    for (uint i = 0; i < numSymbols; i++)
    {
        x = DrawChar(x, y, text[i]);
        x += Font::GetSpacing();
    }

    return x;
}


void Text::DrawBig(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint numSymbols = std::strlen(text);

    for (uint i = 0; i < numSymbols; i++)
    {
        x = DrawBigChar(x, y, sizeOfType, text[i]);
        x += sizeOfType;
    }
}


static int DrawChar(int eX, int eY, char s)
{
    uint8 symbol = static_cast<uint8>(s);

    int8 width = static_cast<int8>(Font::GetWidth(symbol));
    int8 height = static_cast<int8>(Font::GetHeight());

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
                    memDC.DrawPoint({ x, y });
                }
                x++;
            }
        }
    }

    return eX + width;
}


static int DrawBigChar(int eX, int eY, int size, char s)
{
    uint8 symbol = static_cast<uint8>(s);

    int8 width = static_cast<int8>(Font::GetWidth(symbol));
    int8 height = static_cast<int8>(Font::GetHeight());

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
                            memDC.DrawPoint({ x + i, y + j });
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}


void MultiHPointLine::Draw(int x, Color color)
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        DrawHPointLine(x, y[i], count, delta);
    }
}


static void DrawHPointLine(int x, int y, int count, int delta)
{
    for(int i = 0; i < count; i++)
    {
        memDC.DrawPoint({ x, y });
        x += delta;
    }
}


void MultiVPointLine::Draw(int y, Color color)
{
    color.SetAsCurrent();

    for (int i = 0; i < numLines; i++)
    {
        DrawVPointLine(x0[i], y, count, delta);
    }
}


static void DrawVPointLine(int x, int y, int count, int delta)
{
    for(int i = 0; i < count; i++)
    {
        memDC.DrawPoint({ x, y });
        y += delta;
    }
}
