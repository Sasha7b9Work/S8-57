#include "defines.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Recorder/Recorder_win.h"


uint8 *HAL_FSMC::addrData0 = nullptr;
uint8 *HAL_FSMC::addrData1 = nullptr;


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


void HAL_FSMC::SetAddrData(uint8 *address0, uint8 *address1)
{
    addrData0 = address0;
    addrData1 = address1;
}
