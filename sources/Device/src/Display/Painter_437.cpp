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
