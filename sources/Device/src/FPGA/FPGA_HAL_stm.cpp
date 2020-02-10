#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/FreqMeter.h"


/// Время зажигания лампочки синхронизации
static uint timeFireTrig = 0;


void FPGA::ReadFlag()
{
    flag.flag = static_cast<uint16>(HAL_BUS::FPGA::Read(RD::FLAG_LO) | (HAL_BUS::FPGA::Read(RD::FLAG_HI) << 8));

    if (flag.TrigReady() && !forcedStart)
    {
        timeFireTrig = TIME_MS;
        Trig::pulse = true;
    }

    if(!flag.TrigReady() && TIME_MS - timeFireTrig > 1000)
    {
        Trig::pulse = false;
    }

    FreqMeter::Update();
}
