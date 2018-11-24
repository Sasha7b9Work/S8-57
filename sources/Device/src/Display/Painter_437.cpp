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

    Message message(4, Command::Paint_SetPoint, (uint16)x, (uint8)y);

    FSMC::WriteToPanel(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColorValue(Color color, uint value)
{
    Message message(6, Command::Paint_SetPalette, color.value, value);

    FSMC::WriteToPanel(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawTesterData(uint8 mode, Color color, uint8 x[240], uint8 y[240])
{
    Message message(Command::Paint_TesterLines, mode, color.value, x, 240, y, 240);

    FSMC::WriteToPanel(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetFont(Font::Type::E typeFont)
{
    Font::SetFont(typeFont);

    Message message(2, Command::Paint_SetFont, (uint8)typeFont);

    FSMC::WriteToPanel(&message);
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
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::FillRegion(int x, int y, int width, int height, Color color)
{
    SetColor(color);

    Message message(7, Command::Paint_FillRegion, (uint16)x, (uint8)y, (uint16)width, (uint8)height);

    FSMC::WriteToPanel(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawLine(int x0, int y0, int x1, int y1, Color color)
{
    SetColor(color);

    Message message(7, Command::Paint_DrawLine, (uint16)x0, (uint8)y0, (uint16)x1, (uint8)y1);

    FSMC::WriteToPanel(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLine(int x, int y0, int y1, Color color)
{
    SetColor(color);

    Message message(5, Command::Paint_DrawVLine, (uint16)x, (uint8)y0, (uint8)y1);

    FSMC::WriteToPanel(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawText(int x, int y, const char *text, Color color)
{
    /// \todo Такую проверку нужно сделать и на приёмной стороне и тогда здесь убрать

    if (*text == 0)
    {
        return x;
    }

    SetColor(color);

    Message message(Command::Paint_DrawText, (uint16)x, (uint8)y, (char *)text);

    FSMC::WriteToPanel(&message);

    return x + 10;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawBigText(int x, int y, uint8 size, const char *text, Color color)
{
    SetColor(color);

    Message message;
    Message::Create::DrawBigText(message, x, y, size, text);

    FSMC::WriteToPanel(&message);
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

    Message message;
    Message::Create::DrawHLine(message, y, x0, x1);

    FSMC::WriteToPanel(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawRectangle(int x, int y, int width, int height, Color color)
{
    SetColor(color);

    Message message;
    Message::Create::DrawRectangle(message, x, y, width, height);

    FSMC::WriteToPanel(&message);
}
