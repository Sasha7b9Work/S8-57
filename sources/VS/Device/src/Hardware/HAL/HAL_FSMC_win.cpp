#include "defines.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Recorder/Recorder_win.h"


void HAL_FSMC::WriteToFPGA8(uint8 *, uint8)
{
}


void HAL_FSMC::WriteToFPGA16(uint8 *, uint16)
{
}


uint8 HAL_FSMC::ReadFromFPGA(const uint8 *address)
{
    if (address == RD::DATA_A || (address == RD::DATA_A + 1) ||
        address == RD::DATA_B || (address == RD::DATA_B + 1))
    {
        return RecorderHAL::ReadData((address == RD::DATA_A) || (address == RD::DATA_A + 1) ? Chan::A : Chan::B);
    }

    return 0;
}
