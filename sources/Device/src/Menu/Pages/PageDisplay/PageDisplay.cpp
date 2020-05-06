#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"


void ENumAverage::Set(ENumAverage::E v)
{
    S_OSCI_ENUM_AVERAGE = v;
    AveragerOsci::SettingChanged();
}


DEF_CHOICE_10( cSmoothing,                                                                                                                                    //--- ДИСПЛЕЙ - Сглаживание ---
    "Сглаж.",
    "Устанавливает количество точек для расчёта сглаженного по соседним точкам сигнала.",
    DISABLE_RU,
    "2 точки",
    "3 точки",
    "4 точки",
    "5 точек",
    "6 точек",
    "7 точек",
    "8 точек",
    "9 точек",
    "10 точек",
    S_DISP_ENUM_SMOOTH, &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_5( cRefreshFPS,                                                                                                                                 //--- ДИСПЛЕЙ - Частота обновл ---
    "Частота обновл",
    "Задаёт максимальное число выводимых в секунду кадров.",
    "25",
    "10",
    "5",
    "2",
    "1",
    S_DISP_ENUM_FPS, &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_4( cType, // -V206                                                                                                                                       //--- ДИСПЛЕЙ - Сетка ---
    "Сетка",
    "Выбор типа сетки",
    "Тип 1",
    "Тип 2",
    "Тип 3",
    "Тип 4",
    S_DISP_TYPE_GRID, &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static void OnChange_AverageNum(bool)
{
    AveragerOsci::SettingChanged();
}

DEF_CHOICE_9( cAverage_Num,                                                                                                                       //--- ДИСПЛЕЙ - УСРЕДНЕНИЕ - Количество ---
    "Усреднение",
    "Задаёт количество последних измерений, по которым производится усреднение.",
    DISABLE_RU,
    "2",
    "4",
    "8",
    "16",
    "32",
    "64",
    "128",
    "256",
    S_OSCI_ENUM_AVERAGE, &PageDisplay::self, Item::Active, OnChange_AverageNum, Choice::AfterDraw
)


DEF_CHOICE_2( cViewMode,                                                                                                                                      //--- ДИСПЛЕЙ - Отображение ---
    "Отображ",
    "Задаёт режим отображения сигнала",
    "Вектор",
    "Точки",
    S_DISP_MAPPING, &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_7( pDisplay,                                                                                                                                                       //--- ДИСПЛЕЙ ---
    "ДИСПЛЕЙ",
    "Содержит настройки отображения дисплея.",
    &cViewMode,
    PageDisplay::Accumulation::self,
    &cAverage_Num,
    &cSmoothing,
    &cType,
    &cRefreshFPS,
    PageDisplay::Settings::self,
    PageName::Display, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDisplay::self = static_cast<const Page *>(&pDisplay);
