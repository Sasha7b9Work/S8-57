#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"


MenuAutoHide::operator MenuAutoHide::E()
{
    return set.disp.menuAutoHide;
}


MenuAutoHide::E &MenuAutoHide::Ref()
{
    return set.disp.menuAutoHide;
}


ENumMinMax::E &ENumMinMax::Ref()
{
    return set.disp.enumMinMax;
}


ENumAverage::E &ENumAverage::Ref()
{
    return set.disp.enumAverage;
}


ENumAccum::E &ENumAccum::Ref()
{
    return set.disp.enumAccum;
}


DisplayMapping::E &DisplayMapping::Ref()
{
    return set.disp.mapping;
}


TypeGrid::E &TypeGrid::Ref()
{
    return set.disp.typeGrid;
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
    ENumSmoothing::Ref(), &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_5( cRefreshFPS,                                                                                                                                 //--- ДИСПЛЕЙ - Частота обновл ---
    "Частота обновл",
    "Задаёт максимальное число выводимых в секунду кадров.",
    "25",
    "10",
    "5",
    "2",
    "1",
    set.disp.ENumSignalsInSec.value, &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cScaleYtype,                                                                                                                                       //--- ДИСПЛЕЙ - Смещение ---
    "Смещение",
    "Задаёт режим удержания смещения по вертикали\n1. \"Напряжение\" - сохраняется наряжение смещения.\n2. \"Деления\" - сохраняется положение "
    "смещения на экране.",
    "Напряжение",
    "Деления",
    set.disp.linkingRShift, &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_4( cType, // -V206                                                                                                                                       //--- ДИСПЛЕЙ - Сетка ---
    "Сетка",
    "Выбор типа сетки",
    "Тип 1",
    "Тип 2",
    "Тип 3",
    "Тип 4",
    TypeGrid::Ref(), &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
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
    ENumAverage::Ref(), &PageDisplay::self, Item::Active, OnChange_AverageNum, Choice::AfterDraw
)


DEF_CHOICE_2( cViewMode,                                                                                                                                      //--- ДИСПЛЕЙ - Отображение ---
    "Отображ",
    "Задаёт режим отображения сигнала",
    "Вектор",
    "Точки",
    DisplayMapping::Ref(), &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_8( pDisplay,                                                                                                                                                       //--- ДИСПЛЕЙ ---
    "ДИСПЛЕЙ",
    "Содержит настройки отображения дисплея.",
    &cViewMode,
    PageDisplay::PageAccumulation::self,
    &cAverage_Num,
    &cSmoothing,
    &cType,
    &cRefreshFPS,
    &cScaleYtype,
    PageDisplay::PageSettings::self,
    PageName::Display, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDisplay::self = static_cast<const Page *>(&pDisplay);
