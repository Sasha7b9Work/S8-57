#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Osci/FrequencyCounter.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase ppFreqMeter;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_FreqMeter_Enable(bool)
{
    FrequencyCounter::Init();
}

static void OnChanged_FreqMeter_Frequency(bool)
{
    FrequencyCounter::LoadFreqSettings();
}

static void OnChanged_FreqMeter_Period(bool)
{
    FrequencyCounter::LoadPeriodSettings();
}


DEF_CHOICE_2( cEnable,                                                                                                                            //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Частотомер ---
    "Частотомер", "Freq meter",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    FREQ_METER_ENABLED, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ModeView()
{
    return FREQ_METER_IS_ENABLED;
}

DEF_CHOICE_2( cModeView,                                                                                                                               //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Режим ---
    "Режим", "Mode",
    "", "",
    "Частота", "Frequency",
    "Период",  "Period",
    FREQ_METER_MODE_VIEW, ppFreqMeter, IsActive_ModeView, Choice::EmptyChange, FuncDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_SettingsFrequency()
{
    return FREQ_METER_IS_ENABLED && FREQ_METER_MODE_VIEW_IS_PERIOD;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_SettingsPeriod()
{
    return FREQ_METER_IS_ENABLED && FREQ_METER_MODE_VIEW_IS_FREQUENCY;
}

DEF_CHOICE_3( cTimeF,                                                                                                                          //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Время счёта F ---
    "Время счёта F", "Time calc F",
    "Позволяет выбрать точность измерения частоты - чем больше время, тем больше точность и больше время измерения",
    "Allows to choose the accuracy of measurement of frequency - the more time, the accuracy more time of measurement is more",
    "100мс", "100ms",
    "1с", "1s",
    "10с", "10ms",
    FREQ_METER_TIMECOUNTING, ppFreqMeter, IsActive_SettingsPeriod, OnChanged_FreqMeter_Frequency, FuncDraw
)


DEF_CHOICE_4( cFreqClc,                                                                                                                        //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Метки времени ---
    "Метки времени", "Timestamps",
    "Выбор частоты следования счётных импульсов",
    "Choice of frequency of following of calculating impulses",
    "100кГц", "10MHz",
    "1МГц", "200MHz",
    "10МГц", "10MHz",
    "100МГц", "100MHz",
    FREQ_METER_FREQ_CLC, ppFreqMeter, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_6( cNumPeriods,                                                                                                                   //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Кол-во периодов ---
    "Кол периодов", "Num periods",
    "Позволяет выбрать точность измерения периода - чем больше время, тем больше точность и больше время измерения",
    "Allows to choose the accuracy of measurement of period - the more time, the accuracy more time of measurement is more",
    "1",      "1",
    "10",     "10",
    "100",    "100",
    "1000",   "1000",
    "10000",  "10000",
    "100000", "100000",
    FREQ_METER_NUM_PERIODS, ppFreqMeter, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageFunction::PageFrequencyCounter::pointer = &ppFreqMeter;

const Choice *PageFunction::PageFrequencyCounter::GetChoiceTimeF()
{
    return (const Choice *)&cTimeF;
}

const Choice *PageFunction::PageFrequencyCounter::GetChoiceFreqClc()
{
    return (const Choice *)&cFreqClc;
}

const Choice *PageFunction::PageFrequencyCounter::GetChoiceNumPeriods()
{
    return (const Choice *)&cNumPeriods;
}

DEF_PAGE_5( ppFreqMeter, // -V641                                                                                                                              //--- ФУНКЦИЯ - ЧАСТОТОМЕР ---
    "ЧАСТОТОМЕР", "FREQ METER",
    "",
    "",
    &cEnable,           ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Частотомер
    &cModeView,         ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Режим
    &cTimeF,            ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Время счёта F
    &cFreqClc,          ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Метки времени
    &cNumPeriods,       ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Кол-во периодов
    Page::Name::Function_FrequencyCounter, PageFunction::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
