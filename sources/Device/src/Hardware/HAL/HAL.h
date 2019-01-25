#pragma once

namespace HAL
{
    namespace RTC_
    {
        void Init();
        PackedTime GetPackedTime();
        bool SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);
        void SetCorrection(int8 correction);
    }
}
