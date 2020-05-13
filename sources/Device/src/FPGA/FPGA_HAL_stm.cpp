#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"


static uint timeFireTrig = 0;   // Время зажигания лампочки синхронизации

extern bool trig_pulse;


void FPGA::ReadFlag()
{
    flag.flag = static_cast<uint16>(HAL_BUS::FPGA::Read(RD::FLAG_LO) | (HAL_BUS::FPGA::Read(RD::FLAG_HI) << 8));

    if (flag.TrigReady() && !forcedStart)
    {
        timeFireTrig = TIME_MS;
        trig_pulse = true;
    }

    if(!flag.TrigReady() && TIME_MS - timeFireTrig > 1000)
    {
        trig_pulse = false;
    }

    FreqMeter::Update();
}
