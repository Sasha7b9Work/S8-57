#include "defines.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/Memory/ExtendedRAM.h"


void ExtRAM::Init()
{
    HAL_PIO::Init(PORT::CS_RAM, PIN::CS_RAM, HMode::Output_PP, HPull::Up);
    HAL_PIO::Set(PORT::CS_RAM, PIN::CS_RAM);
}
