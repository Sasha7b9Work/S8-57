#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/FreqMeter.h"


void FPGA::ReadFlag()
{
    flag.flag = static_cast<uint16>(HAL_FSMC::ReadFromFPGA(RD::FLAG_LO) | (HAL_FSMC::ReadFromFPGA(RD::FLAG_HI) << 8));

    if (flag.TrigReady() && !givingStart)
    {
        Trig::pulse = true;
    }

    FreqMeter::Update();
}
