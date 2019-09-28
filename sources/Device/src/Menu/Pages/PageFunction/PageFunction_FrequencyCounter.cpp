#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Osci/FrequencyCounter.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_Enable(bool)
{
    FrequencyCounter::Init();
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
    FREQ_METER_ENABLED, &PageFrequencyCounter::self, Item::Active, OnChanged_Enable, Choice::AfterDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ModeView()
{
    return FREQ_METER_IS_ENABLED;
}

static void OnChanged_ModeView(bool)
{
    PageFrequencyCounter::Init();
}

DEF_CHOICE_2( cModeView,                                                                                                                               //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Режим ---
    "Режим",
    "",
    "Частота",
    "Период",
    FREQ_METER_MODE_VIEW, &PageFrequencyCounter::self, IsActive_ModeView, OnChanged_ModeView, Choice::AfterDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_SettingsFrequency()
{
    return FREQ_METER_IS_ENABLED && FREQ_METER_MODE_VIEW_IS_PERIOD;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_TimeF()
{
    return FREQ_METER_IS_ENABLED && FREQ_METER_MODE_VIEW_IS_FREQUENCY;
}

static void OnChanged_TimeF(bool)
{
    FrequencyCounter::LoadFreqSettings();
}

DEF_CHOICE_3( cTimeF,                                                                                                                          //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Время счёта F ---
    "Время счёта F",
    "Позволяет выбрать точность измерения частоты - чем больше время, тем больше точность и больше время измерения",
    "100мс",
    "1с",
    "10с",
    FREQ_METER_TIMECOUNTING, &PageFrequencyCounter::self, IsActive_TimeF, OnChanged_TimeF, Choice::AfterDraw
)


DEF_CHOICE_4( cFreqClc,                                                                                                                        //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Метки времени ---
    "Метки времени",
    "Выбор частоты следования счётных импульсов",
    "100кГц",
    "1МГц",
    "10МГц",
    "100МГц",
    FREQ_METER_FREQ_CLC, &PageFrequencyCounter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, Choice::AfterDraw
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
    FREQ_METER_NUM_PERIODS, &PageFrequencyCounter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, Choice::AfterDraw
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

DEF_PAGE_5_VAR( pFreqMeter, // -V641                                                                                                                               //--- ФУНКЦИЯ - ЧАСТОТОМЕР ---
    "ЧАСТОТОМЕР",
    "",
    &cEnable,           ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Частотомер
    &cModeView,         ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Режим
    &Item::empty,       ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Время счёта F / Метки времени
    &Item::empty,       ///< ИЗМЕРЕНИЯ - ЧАСТОТОМЕР - Кол/во периодов
    &Item::empty,
    PageName::Function_FrequencyCounter, &PageFunction::self, Item::Active, Choice::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageFrequencyCounter::self = (const Page *)&pFreqMeter;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageFrequencyCounter::Init()
{
    Page *page = (Page *)PageFrequencyCounter::self;

    Item **items = (Item **)page->OwnData()->items;

    if (FREQ_METER_MODE_VIEW_IS_FREQUENCY)
    {
        items[2] = (Item *)&cTimeF;
        items[3] = &Item::empty;
    }
    else if (FREQ_METER_MODE_VIEW_IS_PERIOD)
    {
        items[2] = (Item *)&cFreqClc;
        items[3] = (Item *)&cNumPeriods;
    }
}
