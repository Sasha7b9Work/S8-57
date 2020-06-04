#pragma once
#include "TypesFPGA.h"


struct ContextFreqMeter
{
    struct GetFlag
    {
        static bool FREQ_IN_PROCESS();
        static bool PERIOD_IN_PROCESS();
    };
};

