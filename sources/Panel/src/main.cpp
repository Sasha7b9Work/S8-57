#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Utils/DecoderPanel.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard.h"
#include <stdlib.h>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    CPU::Init();
    Display::Init();
    Keyboard::Init();
    FSMC::Init();

    whi le(1)
    {
        uint8 data = FSMC::ReadByte();
        Decoder::AddData(data);
        FSMC::WriteBuffer();
    }
}
