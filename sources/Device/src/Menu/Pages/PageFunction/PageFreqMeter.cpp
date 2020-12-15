#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"


static void OnChanged_Enable(bool)
{
    FreqMeter::Init();
}

static void OnChanged_FreqMeter_Period(bool)
{
    FreqMeter::LoadPeriodSettings();
}


DEF_CHOICE_2( cEnable,                                                                                                                            //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Частотомер ---
    "Частотомер",
    "",
    DISABLE_RU,
    ENABLE_RU,
    S_FREQ_METER_ENABLED, &PageFreqMeter::self, Item::Active, OnChanged_Enable, Choice::AfterDraw
)



static bool IsActive_ModeView()
{
    return S_FREQ_METER_ENABLED;
}

static void OnChanged_ModeView(bool)
{
    PageFreqMeter::Init();
}

DEF_CHOICE_2( cModeMeasure,                                                                                                                               //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Режим ---
    "Режим",
    "",
    "Частота",
    "Период",
    S_FREQ_MODE_MEASURE, &PageFreqMeter::self, IsActive_ModeView, OnChanged_ModeView, Choice::AfterDraw
)



static bool IsActive_SettingsFrequency()
{
    return S_FREQ_METER_ENABLED && S_FREQ_MODE_MEASURE_IS_PERIOD;
}


static bool IsActive_TimeF()
{
    return S_FREQ_METER_ENABLED && S_FREQ_MODE_MEASURE_IS_FREQUENCY;
}

static void OnChanged_TimeF(bool)
{
    FreqMeter::LoadFreqSettings();
}

DEF_CHOICE_3( cTimeF,                                                                                                                          //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Время счёта F ---
    "Время счёта F",
    "Позволяет выбрать точность измерения частоты - чем больше время, тем больше точность и больше время измерения",
    "100мс",
    "1с",
    "10с",
    S_FREQ_TIME_COUNTING, &PageFreqMeter::self, IsActive_TimeF, OnChanged_TimeF, Choice::AfterDraw
)


DEF_CHOICE_4( cFreqClc,                                                                                                                        //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Метки времени ---
    "Метки времени",
    "Выбор частоты следования счётных импульсов",
    "100кГц",
    "1МГц",
    "10МГц",
    "100МГц",
    S_FREQ_FREQ_CLC, &PageFreqMeter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, Choice::AfterDraw
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
    S_FREQ_NUMBER_PERIODS, &PageFreqMeter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, Choice::AfterDraw
)


DEF_PAGE_5_VAR( pFreqMeter,                                                                                                                                    //--- ФУНКЦИЯ - ЧАСТОТОМЕР ---
    "ЧАСТОТОМЕР",
    "",
    &cEnable,
    &cModeMeasure,
    &Item::empty,
    &Item::empty,
    &Item::empty,
    PageName::FreqMeter, &PageFunction::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageFreqMeter::self = static_cast<const Page *>(&pFreqMeter);



void PageFreqMeter::Init()
{
    Page *page = const_cast<Page *>(PageFreqMeter::self);

    Item **items = const_cast<Item **>(page->OwnData()->items);

    if (S_FREQ_MODE_MEASURE_IS_FREQUENCY)
    {
        items[2] = const_cast<Choice *>(&cTimeF);
        items[3] = &Item::empty;
    }
    else if (S_FREQ_MODE_MEASURE_IS_PERIOD) //-V2516
    {
        items[2] = const_cast<Choice *>(&cFreqClc);
        items[3] = const_cast<Choice *>(&cNumPeriods);
    }
}
