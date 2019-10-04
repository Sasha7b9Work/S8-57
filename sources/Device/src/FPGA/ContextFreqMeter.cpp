#include "defines.h"
#include "FPGA/ContextFreqMeter.h"
#include "FPGA/FPGA.h"


bool ContextFreqMeter::GetFlag::FREQ_READY()
{
    return FPGA::GetFlag::FREQ_READY();
}

bool ContextFreqMeter::GetFlag::PERIOD_READY()
{
    return FPGA::GetFlag::PERIOD_READY();
}

bool ContextFreqMeter::GetFlag::FREQ_OVERFLOW()
{
    return FPGA::GetFlag::FREQ_OVERFLOW();
}

bool ContextFreqMeter::GetFlag::PERIOD_OVERFLOW()
{
    return FPGA::GetFlag::PERIOD_OVERFLOW();
}

bool ContextFreqMeter::GetFlag::FREQ_IN_PROCESS()
{
    return FPGA::GetFlag::FREQ_IN_PROCESS();
}

bool ContextFreqMeter::GetFlag::PERIOD_IN_PROCESS()
{
    return FPGA::GetFlag::PERIOD_IN_PROCESS();
}
