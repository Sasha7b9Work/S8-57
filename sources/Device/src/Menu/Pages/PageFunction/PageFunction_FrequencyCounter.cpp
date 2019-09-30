#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Osci/FrequencyCounter.h"
#include "Settings/Settings.h"



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
    set.freq.enabled, &PageFreqMeter::self, Item::Active, OnChanged_Enable, Choice::AfterDraw
)



static bool IsActive_ModeView()
{
    return (set.freq.enabled == FreqMeterEnabled::On);
}

static void OnChanged_ModeView(bool)
{
    PageFreqMeter::Init();
}

DEF_CHOICE_2( cModeView,                                                                                                                               //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Режим ---
    "Режим",
    "",
    "Частота",
    "Период",
    set.freq.modeView, &PageFreqMeter::self, IsActive_ModeView, OnChanged_ModeView, Choice::AfterDraw
)



static bool IsActive_SettingsFrequency()
{
    return (set.freq.enabled == FreqMeterEnabled::On) && (set.freq.modeView == FreqMeterModeView::Period);
}


static bool IsActive_TimeF()
{
    return (set.freq.enabled == FreqMeterEnabled::On) && (set.freq.modeView == FreqMeterModeView::Frequency);
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
    set.freq.timeCounting, &PageFreqMeter::self, IsActive_TimeF, OnChanged_TimeF, Choice::AfterDraw
)


DEF_CHOICE_4( cFreqClc,                                                                                                                        //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Метки времени ---
    "Метки времени",
    "Выбор частоты следования счётных импульсов",
    "100кГц",
    "1МГц",
    "10МГц",
    "100МГц",
    set.freq.freqClc, &PageFreqMeter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, Choice::AfterDraw
)


DEF_CHOICE_6( cNumPeriods,                                                                                                                   //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Кол-во периодов ---
    "Кол периодов",
    "Позволяет выбрать точность измерения периода - чем больше время, тем больше точность и больше время измерения",
    "1",
    "10",
    "100",
    "1000",
    "10000",
    "100000",
    set.freq.numberPeriods, &PageFreqMeter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, Choice::AfterDraw
)



const Choice *PageFreqMeter::GetChoiceTimeF()
{
    return (const Choice *)&cTimeF;
}

const Choice *PageFreqMeter::GetChoiceFreqClc()
{
    return (const Choice *)&cFreqClc;
}

const Choice *PageFreqMeter::GetChoiceNumPeriods()
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
    PageName::Function_FrequencyCounter, &PageFunction::self, Item::Active, Choice::Changed, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageFreqMeter::self = (const Page *)&pFreqMeter;



void PageFreqMeter::Init()
{
    Page *page = (Page *)PageFreqMeter::self;

    Item **items = (Item **)page->OwnData()->items;

    if (set.freq.modeView == FreqMeterModeView::Frequency)
    {
        items[2] = (Item *)&cTimeF;
        items[3] = &Item::empty;
    }
    else if (set.freq.modeView == FreqMeterModeView::Period)
    {
        items[2] = (Item *)&cFreqClc;
        items[3] = (Item *)&cNumPeriods;
    }
    else
    {
        // здесь ничего
    }
}
