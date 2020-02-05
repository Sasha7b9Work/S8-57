#include "defines.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/Memory/ExtRAM.h"


uint8 *ExtRAM::Begin()
{
    return reinterpret_cast<uint8 *>(0x68000000U);
}


uint8 *ExtRAM::End()
{
    return Begin() + 512 * 1024;
}
