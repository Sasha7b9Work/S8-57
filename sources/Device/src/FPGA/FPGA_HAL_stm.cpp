#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/FrequencyCounter.h"


using namespace Address;

extern bool givingStart;


void FPGA::ReadFlag()
{
    flag = (uint16)(HAL_FSMC::ReadFromFPGA(RD::FLAG_LO) | (HAL_FSMC::ReadFromFPGA(RD::FLAG_HI) << 8));

    if (GetFlag::TRIG_READY() && !givingStart)
    {
        Trig::pulse = true;
    }

    FrequencyCounter::Update();
}
