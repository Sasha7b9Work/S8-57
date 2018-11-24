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


DEF_CHOICE_2(cFreqMeter_Enable,                                                                                                                   //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Частотомер ---
    "Частотомер", "Freq meter",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    (FREQ_METER_ENABLED.value), ppFreqMeter, FuncActive, OnChanged_FreqMeter_Enable, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3(cFreqMeter_TimeF,                                                                                                                 //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Время счёта F ---
    "Время счёта F", "Time calc F",
    "Позволяет выбрать точность измерения частоты - чем больше время, тем больше точность и больше время измерения",
    "Allows to choose the accuracy of measurement of frequency - the more time, the accuracy more time of measurement is more",
    "100мс", "100ms",
    "1с", "1s",
    "10с", "10ms",
    FREQ_METER_TIMECOUNTING, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Frequency, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(cFreqMeter_FreqClc,                                                                                                               //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Метки времени ---
    "Метки времени", "Timestamps",
    "Выбор частоты следования счётных импульсов",
    "Choice of frequency of following of calculating impulses",
    "100кГц", "10MHz",
    "1МГц", "200MHz",
    "10МГц", "10MHz",
    "100МГц", "100MHz",
    FREQ_METER_FREQ_CLC, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Period, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_6(cFreqMeter_NumPeriods,                                                                                                          //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Кол-во периодов ---
    "Кол периодов", "Num periods",
    "Позволяет выбрать точность измерения периода - чем больше время, тем больше точность и больше время измерения",
    "Allows to choose the accuracy of measurement of period - the more time, the accuracy more time of measurement is more",
    "1",      "1",
    "10",     "10",
    "100",    "100",
    "1000",   "1000",
    "10000",  "10000",
    "100000", "100000",
    FREQ_METER_NUM_PERIODS, ppFreqMeter, FuncActive, OnChanged_FreqMeter_Period, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageFunction::PageFrequencyCounter::pointer = &ppFreqMeter;

const Choice *PageFunction::PageFrequencyCounter::GetChoiceTimeF()
{
    return (const Choice *)&cFreqMeter_TimeF;
}

const Choice *PageFunction::PageFrequencyCounter::GetChoiceFreqClc()
{
    return (const Choice *)&cFreqMeter_FreqClc;
}

const Choice *PageFunction::PageFrequencyCounter::GetChoiceNumPeriods()
{
    return (const Choice *)&cFreqMeter_NumPeriods;
}

DEF_PAGE_4(ppFreqMeter,                                                                                                                                        //--- ФУНКЦИЯ - ЧАСТОТОМЕР ---
    "ЧАСТОТОМЕР", "FREQ METER",
    "",
    "",
    &cFreqMeter_Enable,      // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Частотомер
    &cFreqMeter_TimeF,       // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Время счёта F
    &cFreqMeter_FreqClc,     // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Метки времени
    &cFreqMeter_NumPeriods,  // ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Кол-во периодов
    Page::Name::Function_FrequencyCounter, PageFunction::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)
