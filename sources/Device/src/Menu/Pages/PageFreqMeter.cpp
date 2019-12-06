#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Osci/FreqMeter.h"
#include "Settings/Settings.h"


FreqMeterEnabled::operator bool()
{
    return (setFreq->enabled == On);
}


bool FreqMeterModeView::IsFrequency()
{
    return (setFreq->modeView == Frequency);
}


bool FreqMeterModeView::IsPeriod()
{
    return (setFreq->modeView == Period);
}


bool FreqMeterTimeCounting::Is100ms()
{
    return (setFreq->timeCounting == _100ms);
}


FreqMeterTimeCounting::operator FreqMeterTimeCounting::E()
{
    return setFreq->timeCounting;
}


FreqMeterFreqClc::operator FreqMeterFreqClc::E()
{
    return setFreq->freqClc;
}


FreqMeterNumberPeriods::operator FreqMeterNumberPeriods::E()
{
    return setFreq->numberPeriods;
}


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
    setFreq->enabled, &PageFreqMeter::self, Item::Active, OnChanged_Enable, Choice::AfterDraw
)



static bool IsActive_ModeView()
{
    return FreqMeterEnabled();
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
    setFreq->modeView, &PageFreqMeter::self, IsActive_ModeView, OnChanged_ModeView, Choice::AfterDraw
)



static bool IsActive_SettingsFrequency()
{
    return FreqMeterEnabled() && FreqMeterModeView::IsPeriod();
}


static bool IsActive_TimeF()
{
    return FreqMeterEnabled() && FreqMeterModeView::IsFrequency();
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
    setFreq->timeCounting, &PageFreqMeter::self, IsActive_TimeF, OnChanged_TimeF, Choice::AfterDraw
)


DEF_CHOICE_4( cFreqClc,                                                                                                                        //--- ФУНКЦИЯ - ЧАСТОТОМЕР - Метки времени ---
    "Метки времени",
    "Выбор частоты следования счётных импульсов",
    "100кГц",
    "1МГц",
    "10МГц",
    "100МГц",
    setFreq->freqClc, &PageFreqMeter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, Choice::AfterDraw
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
    setFreq->numberPeriods, &PageFreqMeter::self, IsActive_SettingsFrequency, OnChanged_FreqMeter_Period, Choice::AfterDraw
)


DEF_PAGE_5_VAR( pFreqMeter,                                                                                                                                    //--- ФУНКЦИЯ - ЧАСТОТОМЕР ---
    "ЧАСТОТОМЕР",
    "",
    &cEnable,
    &cModeView,
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

    if (FreqMeterModeView::IsFrequency())
    {
        items[2] = const_cast<Choice *>(&cTimeF);
        items[3] = &Item::empty;
    }
    else if (FreqMeterModeView::IsPeriod())
    {
        items[2] = const_cast<Choice *>(&cFreqClc);
        items[3] = const_cast<Choice *>(&cNumPeriods);
    }
    else
    {
        // здесь ничего
    }
}
