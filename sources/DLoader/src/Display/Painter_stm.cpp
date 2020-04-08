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
