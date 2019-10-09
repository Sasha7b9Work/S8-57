#include "defines.h"
#include "Command.h"
#include "Transceiver.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Utils/Buffer.h"


void Painter::Init()
{
    Timer::SetAndEnable(TypeTimer::FlashDisplay, Color::OnTimerFlashDisplay, 500);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::BeginScene(Color color)
{
    color.SetAsCurrent();

    Transceiver::Send(Command::Paint_BeginScene);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene()
{
    Transceiver::Send(Command::Paint_EndScene);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawTesterData(uint8 mode, Color color, const uint16 *x, const uint8 *y)
{
    Buffer buffer(3 + TESTER_NUM_POINTS * 2 + TESTER_NUM_POINTS);
    buffer.data[0] = Command::Paint_TesterLines;
    buffer.data[1] = mode;
    buffer.data[2] = color.value;

    uint16 *pointer16 = (uint16 *)(buffer.data + 3);
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        *pointer16++ = x[i];
    }

    uint8 *pointer8 = (uint8 *)(pointer16);
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        *pointer8++ = y[i];
    }

    Transceiver::Send(buffer.data, buffer.size);
}

