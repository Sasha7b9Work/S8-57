#pragma once
#include "Hardware/stm32/stm437.h"
#include "Data/DataSettings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RTC_GET_PACKED_TIME()                       CPU::RTC_::GetPackedTime()
#define RTC_SET_TIME_AND_DATA(d, m, y, h, min, s)   CPU::RTC_::SetTimeAndData(d, m, y, h, min, s)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CPU : public STM437
{
public:

    static void Init();

    class RTC_
    {
    friend class CPU;

    public:
        static PackedTime GetPackedTime();
        static bool SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);
        static void SetCorrection(int8 correction);
    };
};
