#pragma once
#include "defines.h"
#include "Data/DataSettings.h"


namespace HAL
{
    void Init();

    struct RTC_
    {
        static void Init();
        static PackedTime GetPackedTime();
        static bool SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);
    };
}
