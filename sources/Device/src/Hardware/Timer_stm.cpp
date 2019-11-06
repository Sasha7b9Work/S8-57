#include "defines.h"
#include "Hardware/Timer.h"


void Timer::SetAndEnable(TypeTimer::E type, pFuncVV func, uint dTms)
{
    Set(type, func, dTms);
    Enable(type);
}
