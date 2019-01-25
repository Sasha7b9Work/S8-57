#pragma once
#include "defines.h"
#include "Data/DataSettings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RTC_SET_TIME_AND_DATA(d, m, y, h, min, s)   HAL::RTC_::SetTimeAndData(d, m, y, h, min, s)


#define ERROR_HANDLER()                             ::HAL::ErrorHandler(__FILE__, __LINE__)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Hardware
{
    class Clock;
}

namespace HAL
{
    void ErrorHandler(const char *, int);

    class RTC_
    {
    friend class Hardware::Clock;
    public:
        static void Init();
        static PackedTime GetPackedTime();
        static bool SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);
    };
}
