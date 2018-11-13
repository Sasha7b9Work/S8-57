#include "defines.h"
#include "Command.h"
#include "Painter.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include <stdlib.h>


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::Init()
{
    Timer::SetAndEnable(Timer::Type::FlashDisplay, OnTimerFlashDisplay, 500);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::BeginScene(Color color)
{
    uint8 buffer[2] = { Command::Paint_BeginScene, color.value };
    FSMC::WriteToPanel(buffer, 2);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene()
{
    uint8 buffer[1] = { Command::Paint_EndScene };
    FSMC::WriteToPanel(buffer, 1);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPoint(int x, int y, Color color)
{
    SetColor(color);
    uint8 buffer[4] = { Command::Paint_SetPoint, (uint8)x, (uint8)(x >> 8), (uint8)y };
    FSMC::WriteToPanel(buffer, 4);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColorValue(Color color, uint value)
{
    uint8 buffer[6] = { Command::Paint_SetPalette, color.value, (uint8)value, (uint8)(value >> 8), (uint8)(value >> 16), (uint8)(value >> 24) };

    FSMC::WriteToPanel(buffer, 6);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawTesterData(uint8 mode, Color color, uint8 x[240], uint8 y[240])
{
    uint8 *buffer = (uint8 *)malloc(483);
    buffer[0] = Command::Paint_TesterLines;
    buffer[1] = mode;
    buffer[2] = color.value;
    uint8 *pointer = buffer + 3;
    for (int i = 0; i < 240; i++)
    {
        *pointer++ = x[i];
    }
    for (int i = 0; i < 240; i++)
    {
        *pointer++ = y[i];
    }
    FSMC::WriteToPanel(buffer, 483);

    free(buffer);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetFont(Font::Type::E typeFont)
{
    Font::SetFont(typeFont);
    uint8 buffer[2] = { Command::Paint_SetFont, (uint8)typeFont };
    FSMC::WriteToPanel(buffer, 2);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColor(Color color)
{
    currentColor = color;
    if (!WriteFlashColor())
    {
        WriteColor(currentColor);
    }
}
