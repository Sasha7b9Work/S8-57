#include "defines.h"
#include "FPGA/FPGA_HAL.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/FrequencyCounter.h"


using namespace HAL::ADDRESSES::FPGA;

extern bool givingStart;


void FPGA::HAL::ReadFlag()
{
    flag = (uint16)(HAL_FSMC::ReadFromFPGA(RD::FLAG_LO) | (HAL_FSMC::ReadFromFPGA(RD::FLAG_HI) << 8));

    if (GetFlag::TRIG_READY() && !givingStart)
    {
        Trig::pulse = true;
    }

    FrequencyCounter::Update();
}
