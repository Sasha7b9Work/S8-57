#include "defines.h"
#include "common/Command.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Buffer.h"


void Painter::BeginScene(Color color)
{
    color.SetAsCurrent();

    HAL_BUS::Panel::Send(Command::Paint_BeginScene);
}


void Painter::EndScene()
{
    HAL_BUS::Panel::Send(Command::Paint_EndScene);
}


void Painter::DrawTesterData(uint8 mode, Color color, const uint16 *x, const uint8 *y)
{
    Buffer buffer(3 + TESTER_NUM_POINTS * 2 + TESTER_NUM_POINTS);
    buffer.data[0] = Command::Paint_TesterLines; //-V2563
    buffer.data[1] = mode; //-V2563
    buffer.data[2] = color.value; //-V2563

    uint16 *pointer16 = reinterpret_cast<uint16 *>(buffer.data + 3); //-V2563
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        *pointer16++ = x[i]; //-V2563
    }

    uint8 *pointer8 = reinterpret_cast<uint8 *>(pointer16);
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        *pointer8++ = y[i]; //-V2563
    }

    HAL_BUS::Panel::Send(buffer.data, buffer.Size());
}

