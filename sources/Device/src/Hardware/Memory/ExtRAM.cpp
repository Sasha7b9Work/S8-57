#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/Memory/ExtRAM.h"
#include <cstring>


uint8 *ExtRAM::Begin()
{
    return reinterpret_cast<uint8 *>(0x68000000U);
}


uint8 *ExtRAM::End()
{
    return Begin() + 512 * 1024;
}


void ExtRAM::Write(uint8 *buffer, uint size, uint8 *address)
{
    HAL_BUS::ConfigureToFSMC();

    std::memcpy(address, buffer, size);
}
