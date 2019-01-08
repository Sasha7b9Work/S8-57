#include "Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/FSMC.h"
#include "Hardware/LTDC.h"
#include "Utils/Math.h"
#include "Colors.h"
#include "Display/Display.h"
#include <math.h>
#include <string.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Установленное в true значение означает, что скриншот нужно заслать в устройство
static int sendingString = -1;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::BeginScene(Color col)
{   
    SetColor(col);

    uint *address = (uint *)Display::GetBuffer();

    uint *end = address + (BUFFER_HEIGHT * BUFFER_WIDTH) / 4;

    uint value = (uint)col.value + (uint)(col.value << 8) + (uint)(col.value << 16) + (uint)(col.value << 24);

    while (address != end)
    {
        *address++ = value;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColorValue(Color color, uint value)
{
    COLOR(color.value) = value;

    LoadPalette();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::LoadPalette()
{
    LTDC_::SetColors(&COLOR(0), Color::NUMBER.value);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene(void)
{
    if (sendingString >= 0)                                               // Если нужно отправить картинку
    {
#define SIZE  (SIZE_STRING + 2)
        uint8 buffer[SIZE] = { Command::Screen, (uint8)sendingString };
        memcpy(buffer + 2, Display::GetBuffer() + sendingString * SIZE_STRING, SIZE_STRING);

        FSMC::WriteBuffer(buffer, SIZE_STRING);

        sendingString++;
        if (sendingString == 120)
        {
            sendingString = -1;
        }
    }

    LTDC_::ToggleBuffers();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawHLine(int y, int x0, int x1, Color col)
{
#ifdef OPEN
    y *= 2;
    x0 *= 2;
    x1 = x1 * 2 + 1;
#endif

    SetColor(col);

    if(x0 > x1)
    {
        Math::Swap(&x0, &x1);
    }

    uint8 *address = Display::GetBuffer() + x0 + y * BUFFER_WIDTH;
    uint8 *end = Display::GetBufferEnd();

    uint8 value = currentColor.value;

    for (int x = x0; x <= x1; ++x)
    {
        if (address >= end)
        {
            break;
        }
        *address++ = value;
    }

#ifdef OPEN
    address = Display::GetBuffer() + x0 + y * BUFFER_WIDTH + BUFFER_WIDTH;

    for (int x = x0; x <= x1; ++x)
    {
        *address++ = value;
    }
#endif
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawLine(int x1, int y1, int x2, int y2, Color col)
{
    SetColor(col);

    if ((x2 - x1) == 0 && (y2 - y1) == 0)
    {
        ++x1;
    }
    int x = x1;
    int y = y1;
    int dx = (int)fabsf((float)(x2 - x1));
    int dy = (int)fabsf((float)(y2 - y1));
    int s1 = Math::Sign(x2 - x1);
    int s2 = Math::Sign(y2 - y1);
    int temp;
    int exchange = 0;
    if (dy > dx)
    {
        temp = dx;
        dx = dy;
        dy = temp;
        exchange = 1;
    }
    int e = 2 * dy - dx;
    int i = 0;
    for (; i <= dx; i++)
    {
        SetPoint(x, y);
        while (e >= 0)
        {
            if (exchange)
            {
                x += s1;
            }
            else
            {
                y += s2;
            }
            e = e - 2 * dx;
        }
        if (exchange)
        {
            y += s2;
        }
        else
        {
            x += s1;
        }
        e = e + 2 * dy;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawRectangle(int x, int y, int width, int height, Color col)
{
    SetColor(col);

    DrawHLine(y, x, x + width);
    DrawHLine(y + height, x, x + width);
    DrawVLine(x, y, y + height);
    DrawVLine(x + width, y, y + height);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLine(int x, int y0, int y1, Color col)
{
    SetColor(col);

#ifdef OPEN
    x *= 2;
    y0 *= 2;
    y1 *= 2;
#endif

    if(y0 > y1)
    {
        Math::Swap(&y0, &y1);
    }

    uint8 *address = Display::GetBuffer() + x + y0 * BUFFER_WIDTH;
    uint8 *end = Display::GetBufferEnd();

    uint8 value = currentColor.value;

    for (int y = y0; y <= y1; ++y)
    {
        if (address > end)
        {
            break;
        }

        *address = value;
#ifdef OPEN
        *(address + 1) = value;
#endif
        address += BUFFER_WIDTH;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::FillRegion(int x, int y, int width, int height, Color col)
{
    SetColor(col);

    for (int i = y; i <= y + height; ++i)
    {
        DrawHLine(i, x, x + width);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColor(Color color)
{
    if (color.value != Color::NUMBER.value)
    {
        currentColor = color;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T>
static void RecountPoints(T *points, float scale)
{
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        *points = (T)(*points * scale);
        points++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawTesterData(uint8 mode, Color color, uint16 x[TESTER_NUM_POINTS], uint8 y[TESTER_NUM_POINTS])
{
    SetColor(color);

    RecountPoints(x, 320.0F / 240.0F);

    RecountPoints(y, 240.0F / 255.0F);

    if(mode == 1)
    {
        for(int i = 1; i < TESTER_NUM_POINTS - 1; i++)
        {
            *(Display::GetBuffer() + y[i] * BUFFER_WIDTH + x[i]) = currentColor.value;
        }
    }
    else
    {
        for(int i = 1; i < TESTER_NUM_POINTS - 1; i++)
        {
            DrawLine(x[i], y[i], x[i + 1], y[i + 1]);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPoint(int x, int y)
{
    if (x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT)
    {
#ifdef OPEN
        FillRegion(x, y, 0, 0);
#else
        *(Display::GetBuffer() + y * BUFFER_WIDTH + x) = currentColor.value;
#endif
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Painter::ReduceBrightness(uint colorValue, float newBrightness)
{
    int red = (int)(R_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(red, 0, 0xff);                                       // -V2516
    int green = (int)(G_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(green, 0, 0xff);                                     // -V2516
    int blue = (int)(B_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(blue, 0, 0xff);                                      // -V2516
    return MAKE_COLOR(red, green, blue);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SendScreenToDevice()
{
    sendingString = 0;
}
