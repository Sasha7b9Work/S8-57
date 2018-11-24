#include "defines.h"
#include "Command.h"
#include "Message.h"
#include "Painter.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Utils/Buffer.h"
#include <stdlib.h>
#include <cstring>


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
    Message message(1, Command::Paint_EndScene);

    FSMC::WriteToPanel(&message);
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
void Painter::DrawTesterData(uint8 mode, Color color, uint8 x[240], uint8 y[240])
{
    Buffer buffer(483);
    buffer.Data()[0] = Command::Paint_TesterLines;
    buffer.Data()[1] = mode;
    buffer.Data()[2] = color.value;
    uint8 *pointer = buffer.Data() + 3;
    for (int i = 0; i < 240; i++)
    {
        *pointer++ = x[i];
    }
    for (int i = 0; i < 240; i++)
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
    volatile float recValue = 1601.0f;
    if (brightness < 100)
    {
        recValue = 64.0f + (600.0f - 63.0f) / 100.0f / 100.0f * brightness * brightness;
    }
    /*
    uint8 command[4] = {SET_BRIGHTNESS};
    WRITE_SHORT(1, recValue);

    SendToDisplay(command, 4);
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::FillRegion(int x, int y, int width, int height, Color color)
{
    SetColor(color);
    uint8 buffer[7] = { Command::Paint_FillRegion, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    FSMC::WriteToPanel(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawLine(int x0, int y0, int x1, int y1, Color color)
{
    SetColor(color);
    uint8 buffer[7] = { Command::Paint_DrawLine, (uint8)x0, (uint8)(x0 >> 8), (uint8)y0, (uint8)x1, (uint8)(x1 >> 8), (uint8)y1 };
    FSMC::WriteToPanel(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLine(int x, int y0, int y1, Color color)
{
    SetColor(color);
    uint8 buffer[5] = { Command::Paint_DrawVLine, (uint8)x, (uint8)(x >> 8), (uint8)y0, (uint8)y1 };
    FSMC::WriteToPanel(buffer, 5);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawText(int x, int y, const char *text, Color color)
{
    /// \todo Такую проверку нужно сделать и на приёмной стороне и тогда здесь убрать

    if (*text == 0)
    {
        return x;
    }

#define MAX_SIZE_BUFFER 100

    if (std::strlen(text) + 1 > MAX_SIZE_BUFFER)
    {
        return x + 10;
    }

    SetColor(color);
    size_t size = (size_t)(1 + 2 + 1 + 1 + std::strlen(text));
    uint8 buffer[MAX_SIZE_BUFFER] = { Command::Paint_DrawText, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)(size - 5) };

    uint8 *pointer = &buffer[5];

    while (*text)
    {
        *pointer++ = (uint8)*text++;
    }

    FSMC::WriteToPanel(buffer, (int)size);

    return x + 10;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawBigText(int eX, int eY, uint8 sizeSymbol, const char *text, Color color)
{
    SetColor(color);

    uint numSymbols = std::strlen(text);
    uint size = 1 + 2 + 1 + 1 + numSymbols + 1;
    uint8 buffer[MAX_SIZE_BUFFER] = { Command::Paint_DrawBigText, (uint8)eX, (uint8)(eX >> 8), (uint8)eY, sizeSymbol, (uint8)(size - 6) };

    uint8 *pointer = &buffer[6];

    while (*text)
    {
        *pointer++ = (uint8)*text++;
    }

    FSMC::WriteToPanel(buffer, (int)size);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawChar(int x, int y, char symbol, Color color)
{
    DrawFormatText(x, y, color, "%c", symbol);

    return 8;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawHLine(int y, int x0, int x1, Color color)
{
    SetColor(color);
    uint8 buffer[6] = { Command::Paint_DrawHLine, (uint8)y, (uint8)x0, (uint8)(x0 >> 8), (uint8)x1, (uint8)(x1 >> 8) };
    FSMC::WriteToPanel(buffer, 6);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawRectangle(int x, int y, int width, int height, Color color)
{
    SetColor(color);
    uint8 buffer[7] = { Command::Paint_DrawRectangle, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    FSMC::WriteToPanel(buffer, 7);
}
