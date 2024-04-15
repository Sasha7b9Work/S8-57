// 2023/11/22 13:37:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "PainterMem.h"
#include <cstdlib>


namespace PainterMem
{
    // Адрес начала области памяти, в которой хранится буфер для рисования. Если == 0, то буфер не создан.
    static uint8 *buffer = 0;
    static uint8 *endBuffer = 0;
    static int    width = 0;
    static int    height = 0;
    static Color  color = Color::FILL;

    // Возвращает адрес байта с координатами x, y.
#define ADDRESS_BYTE(x, y) (buffer + ((y) * width + (x)))

#define SET_POINT(x, y)                             \
    uint8 *address = ADDRESS_BYTE(x, y);            \
    if (address >= buffer && address < endBuffer)   \
    {                                               \
        *address = color.value;                     \
    }
}


uint8 *PainterMem::CreateBuffer(int w, int h)
{
    if (buffer == 0)        // Выделяем память только если она не была освобождена вызовом DeleteBuffer()
    {
        width = w;
        height = h;
        buffer = static_cast<uint8 *>(std::malloc(static_cast<uint>(width * height)));
        if (buffer != nullptr)
        {
            endBuffer = buffer + width * height;
        }
        else
        {
            endBuffer = nullptr;
        }
    }

    return buffer;
}


void PainterMem::DeleteBuffer()
{
    std::free(buffer);
    buffer = 0;
}


void PainterMem::SetPoint(int x, int y, Color col)
{
    color = col;

    uint8 *address = ADDRESS_BYTE(x, y);

    if (address >= buffer && address < endBuffer)
    {
        *address = color.value;
    }
}


void PainterMem::SetPoint(int x, int y)
{
    uint8 *address = ADDRESS_BYTE(x, y);

    if(address >= buffer && address < endBuffer)
    {
        *address = color.value;
    }
}


void PainterMem::FillRect(int x, int y, int w, int h, Color col)
{
    if(col != Color::NUMBER)
    {
        color = col;
    }

    for(int i = 0; i <= h; i++)
    {
        DrawHLine(y + i, x, x + w);
    }
}


void PainterMem::DrawVLine(int x, int y0, int y1, Color col)
{
    if(col != Color::NUMBER)
    {
        color = col;
    }

    for (int y = y0; y <= y1; y++)
    {
        SET_POINT(x, y);
    }
}


void PainterMem::DrawHLine(int y, int x0, int x1, Color col)
{
    if(col != Color::NUMBER)
    {
        color = col;
    }

    for(int x = x0; x <= x1; x++)
    {
        SET_POINT(x, y);
    }
}


void PainterMem::DrawRectangle(int x, int y, int w, int h, Color col)
{
    if(col != Color::NUMBER)
    {
        color = col;
    }

    DrawVLine(x, y, y + h);
    DrawVLine(x + w, y, y + h);
    DrawHLine(y, x, x + w);
    DrawHLine(y + h, x, x + w);
}
