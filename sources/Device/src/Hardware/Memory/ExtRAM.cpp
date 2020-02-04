#include "defines.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/Memory/ExtRAM.h"


void ExtRAM::Init()
{
    HAL_PIO::Init(PIN_CS_RAM, HMode::Output_PP, HPull::Up);
    HAL_PIO::Set(PIN_CS_RAM);
}
