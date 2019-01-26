#include "defines.h"
#include "log.h"
#include "Command.h"
#include "Message.h"
#include "Painter.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Tester/Tester.h"
#include "Utils/Buffer.h"
#include <stdlib.h>
#include <cstring>


using HAL::FSMC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool needSaveScreen = false;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::Init()
{
    Timer::SetAndEnable(Timer::Type::FlashDisplay, Color::OnTimerFlashDisplay, 500);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::BeginScene(Color color)
{
    Message message(2, Command::Paint_BeginScene, color.value);

    FSMC::WriteToPanel(message.Data(), message.Size());
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
void Painter::DrawTesterData(uint8 mode, Color color, const uint8 *x, const uint8 *y)
{
    Buffer buffer(483);
    buffer.data[0] = Command::Paint_TesterLines;
    buffer.data[1] = mode;
    buffer.data[2] = color.value;
    uint8 *pointer = buffer.data + 3;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        *pointer++ = x[i];
    }
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        *pointer++ = y[i];
    }
    FSMC::WriteToPanel(buffer.data, 483);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SaveScreenToDrive()
{
    needSaveScreen = true;
}
