#include "defines.h"
#include "FSMC.h"
#include "Timer.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FSMC::WriteToFPGA16(uint8 *address, uint16 value)
{
    if (mode != ModeFPGA)
    {
        ConfigureForFPGA();
    }

    PAUSE_ON_TICKS(100);    /// \todo Без этой строки замедлен вывод при включённой оптимизации и TBase >= 0.5мс

    *address = (uint8)value;
    *(address + 1) = (uint8)(value >> 8);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::WriteToFPGA8(uint8 *address, uint8 value)
{
    if (mode != ModeFPGA)
    {
        ConfigureForFPGA();
    }

    *address = value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 FSMC::ReadFromFPGA(const uint8 *address)
{
    if (mode != ModeFPGA)
    {
        ConfigureForFPGA();
    }

    return *address;
}
