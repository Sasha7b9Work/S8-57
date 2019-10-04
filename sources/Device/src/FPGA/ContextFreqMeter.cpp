#include "defines.h"
#include "FPGA/ContextFreqMeter.h"
#include "FPGA/FPGA.h"


bool ContextFreqMeter::GetFlag::FREQ_READY()
{
    return FPGA::flag.FreqReady();
}

bool ContextFreqMeter::GetFlag::PERIOD_READY()
{
    return FPGA::flag.PeriodReady();
}

bool ContextFreqMeter::GetFlag::FREQ_OVERFLOW()
{
    return FPGA::flag.FreqOverflow();
}

bool ContextFreqMeter::GetFlag::PERIOD_OVERFLOW()
{
    return FPGA::flag.PeriodOverflow();
}

bool ContextFreqMeter::GetFlag::FREQ_IN_PROCESS()
{
    return FPGA::flag.FreqInProcess();
}

bool ContextFreqMeter::GetFlag::PERIOD_IN_PROCESS()
{
    return FPGA::flag.PeriodInProcess();
}
