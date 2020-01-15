#include "defines.h"
#include "Hardware/Timer.h"


void Timer::PauseOnTime(uint timeMS)
{
    HAL_Delay(timeMS);
}
