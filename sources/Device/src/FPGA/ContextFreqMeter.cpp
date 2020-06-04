#include "defines.h"
#include "FPGA/ContextFreqMeter.h"
#include "FPGA/FPGA.h"


bool ContextFreqMeter::GetFlag::PERIOD_IN_PROCESS()
{
    return FPGA::flag.PeriodInProcess();
}
