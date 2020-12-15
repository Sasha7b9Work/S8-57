#include "defines.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"


static void OnChanged_Mode(bool)
{
    Osci::ChangedTrigStartMode();
}

DEF_CHOICE_3( cMode, // -V206                                                                                                                                         //--- СИНХР - Режим ---
    "Режим"
    ,
    "Задаёт режим запуска:\n"
    "1. \"Авто\" - запуск происходит автоматически.\n"
    "2. \"Ждущий\" - запуск происходит по уровню синхронизации.\n"
    "3. \"Однократный\" - запуск происходит по достижении уровня синхронизации один раз. Для следующего измерения нужно нажать кнопку ПУСК/СТОП."
    ,
    "Авто ",
    "Ждущий",
    "Однократный",
    S_TRIG_START_MODE, &PageTrig::self, Item::Active, OnChanged_Mode, Choice::AfterDraw
)


static void OnChanged_Source(bool)
{
    TrigSource::Set();
}

DEF_CHOICE_2( cSource, // -V206                                                                                                                                    //--- СИНХР - Источник ---
    "Источник",
    "Выбор источника сигнала синхронизации."
    ,
    "Канал 1",
    "Канал 2",
    S_TRIG_SOURCE, &PageTrig::self, Item::Active, OnChanged_Source, Choice::AfterDraw
)


static void OnChanged_Polarity(bool)
{
    TrigPolarity().Load();
}

DEF_CHOICE_2( cPolarity, //-V206                                                                                                                                 //--- СИНХР - Полярность ---
    "Полярность"
    ,
    "1. \"Фронт\" - запуск происходит по фронту синхроимпульса.\n"
    "2. \"Срез\" - запуск происходит по срезу синхроимпульса."
    ,
    "Фронт",
    "Срез",
    S_TRIG_POLARITY, &PageTrig::self, Item::Active, OnChanged_Polarity, Choice::AfterDraw
)


static void OnChanged_Filtr(bool)
{
    TrigInput().Load();
}

DEF_CHOICE_3( cFiltr, // -V206                                                                                                                                         //--- СИНХР - Вход ---
    "Фильтр"
    ,
    "Выбор фильтра на источнике синхронизации:\n"
    "1. \"ПС\" - полный сигнал.\n"
    "2. \"ФНЧ\" - фильтр низких частот.\n"
    "3. \"ФВЧ\" - фильтр высоких частот."
    ,
    "ПС",
    "НЧ",
    "ВЧ",
    S_TRIG_INPUT, &PageTrig::self, Item::Active, OnChanged_Filtr, Choice::AfterDraw
)


static void OnPress_Search()
{
    TrigLevel::Find();
}

DEF_BUTTON(bSearch,                                                                                                                                          //--- СИНХР - ПОИСК - Найти ---
    "Поиск",
    "Производит поиск уровня синхронизации.",
    &PageTrig::self, Item::Active, OnPress_Search
)


DEF_PAGE_5( pTrig,                                                                                                                                                            //--- СИНХР --- //-V2567
    "СИНХР",
    "Содержит настройки синхронизации.",
    &cMode,
    &cSource,
    &cPolarity,
    &cFiltr,
    &bSearch,
    PageName::Trig, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageTrig::self = static_cast<const Page *>(&pTrig);
