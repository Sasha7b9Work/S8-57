#include <stm32f4xx_hal.h>
#include "defines.h"
#include "CPU.h"
#include "Hardware/Timer.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define VALUE_FOR_RTC 0x644


#define RTC_CLOCK_SOURCE_LSE
// #define RTC_CLOCK_SOURCE_LSI


#ifdef RTC_CLOCK_SOURCE_LSI
#define RTC_ASYNCH_PREDIV 0x7f
#define RTC_SYNCH_PREDIV 0x0130
#endif

#ifdef RTC_CLOCK_SOURCE_LSE
#define RTC_ASYNCH_PREDIV 0x7f
#define RTC_SYNCH_PREDIV 0x00ff
#endif

static RTC_HandleTypeDef handleRTC =
{
    RTC,
    {
        RTC_HOURFORMAT_24,          // HourFormat
        RTC_ASYNCH_PREDIV,          // AsynchPrediv
        RTC_SYNCH_PREDIV,           // SynchPrediv
        RTC_OUTPUT_DISABLE,         // OutPut
        RTC_OUTPUT_POLARITY_HIGH,   // OutPutPolarity
        RTC_OUTPUT_TYPE_OPENDRAIN   // OutPutType
    }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::Init()
{
    RTC_::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::RTC_::Init()
{
    RCC_OscInitTypeDef        RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    __HAL_RCC_RTC_ENABLE();

    HAL_StatusTypeDef status = HAL_RTC_Init((RTC_HandleTypeDef*)&handleRTC);

    if (status != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_RTCEx_BKUPRead((RTC_HandleTypeDef*)&handleRTC, RTC_BKP_DR0) != VALUE_FOR_RTC)
    {
        if (SetTimeAndData(11, 11, 11, 11, 11, 11))
        {
            HAL_RTCEx_BKUPWrite((RTC_HandleTypeDef*)&handleRTC, RTC_BKP_DR0, VALUE_FOR_RTC);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::RTC_::SetCorrection(int8)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PackedTime CPU::RTC_::GetPackedTime()
{
    PackedTime time;

    RTC_TimeTypeDef isTime;

    HAL_RTC_GetTime((RTC_HandleTypeDef*)&handleRTC, &isTime, FORMAT_BIN);

    time.hours = isTime.Hours;
    time.minutes = isTime.Minutes;
    time.seconds = isTime.Seconds;

    RTC_DateTypeDef isDate;
    HAL_RTC_GetDate((RTC_HandleTypeDef*)&handleRTC, &isDate, FORMAT_BIN);

    time.year = isDate.Year;
    time.month = isDate.Month;
    time.day = isDate.Date;

    time.timeMS = TIME_MS;

    return time;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool CPU::RTC_::SetTimeAndData(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds)
{
    RTC_DateTypeDef dateStruct;
    dateStruct.WeekDay = RTC_WEEKDAY_MONDAY;
    dateStruct.Month = (uint8)month;
    dateStruct.Date = (uint8)day;
    dateStruct.Year = (uint8)year;

    if (HAL_RTC_SetDate((RTC_HandleTypeDef*)&handleRTC, &dateStruct, FORMAT_BIN) != HAL_OK)
    {
        return false;
    };

    RTC_TimeTypeDef timeStruct;
    timeStruct.Hours = (uint8)hours;
    timeStruct.Minutes = (uint8)minutes;
    timeStruct.Seconds = (uint8)seconds;
    timeStruct.TimeFormat = RTC_HOURFORMAT_24;
    timeStruct.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    timeStruct.StoreOperation = RTC_STOREOPERATION_SET;

    if (HAL_RTC_SetTime((RTC_HandleTypeDef*)&handleRTC, &timeStruct, FORMAT_BIN) != HAL_OK)
    {
        return false;
    };

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::ErrorHandler(const char *, int)
{
    while (true)  { }
}
