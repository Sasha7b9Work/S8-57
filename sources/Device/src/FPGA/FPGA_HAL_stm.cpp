#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/FreqMeter.h"


void FPGA::ReadFlag()
{
    flag.flag = static_cast<uint16>(HAL_BUS::ReadFromFPGA(RD::FLAG_LO) | (HAL_BUS::ReadFromFPGA(RD::FLAG_HI) << 8));

    if (flag.TrigReady() && !forcedStart)
    {
        Trig::pulse = true;
    }

    FreqMeter::Update();
}
