#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Osci/FrequencyCounter.h"
#include "Settings/Settings.h"


extern const Page ppFreqMeter;

const Page * const PageFrequencyCounter::self = (const Page *)&ppFreqMeter;


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
    "Частотомер",
    "",
    DISABLE_RU,
    ENABLE_RU,
    FREQ_METER_ENABLED, &PageFrequencyCounter::self, 0, OnChanged_FreqMeter_Enable, 0
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ModeView()
{
    return FREQ_METER_IS_ENABLED;
}

DEF_CHOICE_2( cModeView,                                                                                                                               //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Режим ---
    "Режим",
    "",
    "Частота",
    "Период",
    FREQ_METER_MODE_VIEW, &PageFrequencyCounter::self, IsActive_ModeView, 0, 0
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
    "Время счёта F",
    "Позволяет выбрать точность измерения частоты - чем больше время, тем больше точность и больше время измерения",
    "100мс",
    "1с",
    "10с",
    FREQ_METER_TIMECOUNTING, &PageFrequencyCounter::self, IsActive_SettingsPeriod, OnChanged_FreqMeter_Frequency, 0
)


DEF_CHOICE_4( cFreqClc,                                                                                                                        //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Метки времени ---
    "Метки времени",
    "Выбор частоты следования счётных импульсов",
    "100кГц",
    "1МГц",
    "10МГц",
    "100МГц",
    FREQ_METER_FREQ_CLC, &PageFrequencyCounter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_6( cNumPeriods,                                                                                                                   //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Кол-во периодов ---
    "Кол периодов",
    "Позволяет выбрать точность измерения периода - чем больше время, тем больше точность и больше время измерения",
    "1",
    "10",
    "100",
    "1000",
    "10000",
    "100000",
    FREQ_METER_NUM_PERIODS, &PageFrequencyCounter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, 0
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Choice *PageFrequencyCounter::GetChoiceTimeF()
{
    return (const Choice *)&cTimeF;
}

const Choice *PageFrequencyCounter::GetChoiceFreqClc()
{
    return (const Choice *)&cFreqClc;
}

const Choice *PageFrequencyCounter::GetChoiceNumPeriods()
{
    return (const Choice *)&cNumPeriods;
}

DEF_PAGE_5( ppFreqMeter, // -V641                                                                                                                              //--- ФУНКЦИЯ - ЧАСТОТОМЕР ---
    "ЧАСТОТОМЕР",
    "",
    &cEnable,           ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Частотомер
    &cModeView,         ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Режим
    &cTimeF,            ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Время счёта F
    &cFreqClc,          ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Метки времени
    &cNumPeriods,       ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Кол-во периодов
    Page::Name::Function_FrequencyCounter, &PageFunction::self, 0, 0, 0, 0
)
