#include "defines.h"
#include "Display/Painter.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Utils/CommonFunctions.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"
#include "Osci/Osci_Averager.h"



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
    set.disp.ENumSmoothing.value, &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
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
    set.disp.typeGrid, &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static void OnChange_AverageNum(bool)
{
    Osci::Averager::SettingChanged();
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
    set.disp.ENumAverage, &PageDisplay::self, Item::Active, OnChange_AverageNum, Choice::AfterDraw
)


DEF_CHOICE_2( cViewMode,                                                                                                                                      //--- ДИСПЛЕЙ - Отображение ---
    "Отображ",
    "Задаёт режим отображения сигнала",
    "Вектор",
    "Точки",
    set.disp.modeDrawSignal, &PageDisplay::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


//_DEF_PAGE_1( pageViewSignal, //-V641
//    "ОТОБРАЖ",
//    "Настройки отображения сигналов",
//    &cViewMode,
//    PageName::Display_View, &PageDisplay::self, Item::Active, E_VB, E_VV, E_BfKE
//)


DEF_PAGE_8( pDisplay, // -V641 // -V1027                                                                                                                                    //--- ДИСПЛЕЙ ---
    "ДИСПЛЕЙ",
    "Содержит настройки отображения дисплея.",
    &cViewMode,
    PageDisplay::PageAccumulation::self,
    &cAverage_Num,
    //PageDisplay::PageAverage::self,
    &cSmoothing,
    &cType,
    &cRefreshFPS,
    //PageDisplay::PageGrid::self,
    &cScaleYtype,
    PageDisplay::PageSettings::self,
//    &cThickness,
    PageName::Display, nullptr, Item::Active, Page::Changed, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDisplay::self = (const Page *)&pDisplay;
