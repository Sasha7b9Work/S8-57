#include "defines.h"
#include "log.h"
#include "Command.h"
#include "Message.h"
#include "Painter.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Tester/Tester.h"
#include "Utils/Buffer.h"
#include <stdlib.h>
#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool needSaveScreen = false;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::Init()
{
    Timer::SetAndEnable(Timer::Type::FlashDisplay, OnTimerFlashDisplay, 500);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::BeginScene(Color color)
{
    Message message(2, Command::Paint_BeginScene, color.value);

    FSMC::WriteToPanel(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene()
{
    uint8 buffer[1] = { Command::Paint_EndScene };
    FSMC::WriteToPanel(buffer, 1);

    if (needSaveScreen)
    {
        needSaveScreen = 0;

        buffer[0] = Command::Screen;
        FSMC::WriteToPanel(buffer, 1);

        uint8 d[10];

        for (int i = 0; i < 10; i++)
        {
            d[i] = FSMC::ReadByteNow();
        }

        LOG_WRITE("%d %d %d %d %d %d %d %d %d %d", d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9]);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPoint(int x, int y, Color color)
{
    SetColor(color);
    uint8 buffer[4] = { Command::Paint_SetPoint, (uint8)x, (uint8)(x >> 8), (uint8)y };
    FSMC::WriteToPanel(buffer, 4);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColorValue(Color color, uint value)
{
    uint8 buffer[6] = { Command::Paint_SetPalette, color.value, (uint8)value, (uint8)(value >> 8), (uint8)(value >> 16), (uint8)(value >> 24) };

    FSMC::WriteToPanel(buffer, 6);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawTesterData(uint8 mode, Color color, const uint8 *x, const uint8 *y)
{
    Buffer buffer(483);
    buffer.Data()[0] = Command::Paint_TesterLines;
    buffer.Data()[1] = mode;
    buffer.Data()[2] = color.value;
    uint8 *pointer = buffer.Data() + 3;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        *pointer++ = x[i];
    }
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        *pointer++ = y[i];
    }
    FSMC::WriteToPanel(buffer.Data(), 483);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetFont(Font::Type::E typeFont)
{
    Font::SetFont(typeFont);
    uint8 buffer[2] = { Command::Paint_SetFont, (uint8)typeFont };
    FSMC::WriteToPanel(buffer, 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SaveScreenToDrive()
{
    needSaveScreen = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColor(Color color)
{
    currentColor = color;
    if (!WriteFlashColor())
    {
        WriteColor(currentColor);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetBrightnessDisplay(int16 brightness)
{
    volatile float recValue = 1601.0F;
    if (brightness < 100)
    {
        recValue = 64.0F + (600.0F - 63.0F) / 100.0F / 100.0F * brightness * brightness;
    }
    /*
    uint8 command[4] = {SET_BRIGHTNESS};
    WRITE_SHORT(1, recValue);

    SendToDisplay(command, 4);
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void Region(int x, int y, int width, int height, Color color)
{
    SetColor(color);
    uint8 buffer[7] = { Command::Paint_FillRegion, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    FSMC::WriteToPanel(buffer, 7);
}
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void Painter::DrawLine(int x0, int y0, int x1, int y1, Color color)
{
    SetColor(color);
    uint8 buffer[7] = { Command::Paint_DrawLine, (uint8)x0, (uint8)(x0 >> 8), (uint8)y0, (uint8)x1, (uint8)(x1 >> 8), (uint8)y1 };
    FSMC::WriteToPanel(buffer, 7);
}
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void Painter::DrawVLine(int x, int y0, int y1, Color color)
{
    SetColor(color);
    uint8 buffer[5] = { Command::Paint_DrawVLine, (uint8)x, (uint8)(x >> 8), (uint8)y0, (uint8)y1 };
    FSMC::WriteToPanel(buffer, 5);
}
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawText(int x, int y, const char *text, Color color) //-V2506
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

    SetColor(color);
    size_t size = (size_t)(1 + 2 + 1 + 1 + std::strlen(text)); //-V2513

    uint8 buffer[MAX_SIZE_BUFFER] = { Command::Paint_DrawText, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)(size - 5) };

    uint8 *pointer = &buffer[5];

    while (*text)
    {
        *pointer++ = (uint8)*text++;
    }

    FSMC::WriteToPanel(buffer, size);

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawBigText(int eX, int eY, uint8 sizeSymbol, const char *text, Color color)
{
    SetColor(color);

    uint numSymbols = std::strlen(text); //-V2513
    uint size = 1 + 2 + 1 + 1 + numSymbols + 1;
    uint8 buffer[MAX_SIZE_BUFFER] = { Command::Paint_DrawBigText, (uint8)eX, (uint8)(eX >> 8), (uint8)eY, sizeSymbol, (uint8)(size - 6) };

    uint8 *pointer = &buffer[6];

    while (*text)
    {
        *pointer++ = (uint8)*text++;
    }

    FSMC::WriteToPanel(buffer, size);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/**/
int Painter::DrawChar(int x, int y, char symbol, Color color)
{
    String("%c", symbol).Draw(x, y, color);

    return x + Font::GetLengthSymbol(symbol) + 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void Painter::DrawHLine(int y, int x0, int x1, Color color)
{
    SetColor(color);
    uint8 buffer[6] = { Command::Paint_DrawHLine, (uint8)y, (uint8)x0, (uint8)(x0 >> 8), (uint8)x1, (uint8)(x1 >> 8) };
    FSMC::WriteToPanel(buffer, 6);
}
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void Rectangle(int x, int y, int width, int height, Color color)
{
    SetColor(color);
    uint8 buffer[7] = { Command::Paint_DrawRectangle, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    FSMC::WriteToPanel(buffer, 7);
}
*/
