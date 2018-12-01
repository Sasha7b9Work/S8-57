#include "stdafx.h"
#ifndef WIN32
#include <stm32f4xx.h>
#include "defines.h"
#include "Display/Painter.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "FPGA/FPGA.h"
#include "Menu/Menu.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageDisplay;

const PageBase *PageDisplay::pointer = &pageDisplay;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cThickness,                                                                                            //--- ДИСПЛЕЙ - Толщина ---
    "Толщина", "Thickness",
    "Позволяет изменять толщину выводимых на экран изображений",
    "Allows you to change the thickness of the signals displayed on the screen",
    "x1", "x1",
    "x3", "x3",
    THICKNESS_SIGNAL, pageDisplay, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageDisplay::Init()
{
    PageSettings::PageColors::OnChanged_Settings_Colors_Background(true);   // Заносим значения в гувернёры цветов
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cViewMode,                                                                                         //--- ДИСПЛЕЙ - Отображение ---
    "Отображ.", "View",
    "Задаёт режим отображения сигнала.",
    "Sets the display mode signal.",
    "Вектор", "Vector",
    "Точки",  "Points",
    MODE_DRAW_SIGNAL, pageDisplay, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_MinMax()
{
    return !IN_RANDOM_MODE && !IN_P2P_MODE;
}

static void OnChanged_MinMax(bool)
{
    /*
    int maxMeasures = DS_NumberAvailableEntries();  
    int numMinMax = sDisplay_NumberMinMax();

    if (maxMeasures < numMinMax)
    {
    display.ShowWarningWithNumber(ExcessValues, maxMeasures);
    }
    */
}

DEF_CHOICE_8(       cMinMax,                                                                                              //--- ДИСПЛЕЙ - Мин Макс ---
    "Мин Макс", "Min Max",
    "Задаёт количество последних измерений, по которым строятся ограничительные линии, огибающие минимумы и максимумы измерений.",
    "Sets number of the last measurements on which the limiting lines which are bending around minima and maxima of measurements are under "
    "construction."
    ,
    DISABLE_RU,DISABLE_EN,
    "2",   "2",
    "4",   "4",
    "8",   "8",
    "16",  "16",
    "32",  "32",
    "64",  "64",
    "128", "128",
    ENUM_MIN_MAX, pageDisplay, IsActive_MinMax, OnChanged_MinMax, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_REG_10(  cSmoothing,                                                                                        //--- ДИСПЛЕЙ - Сглаживание ---
    "Сглаж.", "Smoothing",
    "Устанавливает количество точек для расчёта сглаженного по соседним точкам сигнала.",
    "Establishes quantity of points for calculation of the signal smoothed on the next points."
    ,
    DISABLE_RU, DISABLE_EN,
    "2 точки",  "2 points",
    "3 точки",  "3 points",
    "4 точки",  "4 points",
    "5 точек",  "5 points",
    "6 точек",  "6 points",
    "7 точек",  "7 points",
    "8 точек",  "8 points",
    "9 точек",  "9 points",
    "10 точек", "10 points",
    ENUM_SMOOTHING, pageDisplay, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageDisplay::OnChanged_RefreshFPS(bool)
{
    FPGA::SetENumSignalsInSec(ENUM_SIGNALS_IN_SEC);
}

DEF_CHOICE_5(       cRefreshFPS,                                                                                    //--- ДИСПЛЕЙ - Частота обновл ---
    "Частота обновл", "Refresh rate",
    "Задаёт максимальное число выводимых в секунду кадров.",
    "Sets the maximum number of the shots removed in a second.",
    "25", "25",
    "10", "10",
    "5",  "5",
    "2",  "2",
    "1",  "1",
    ENUM_SIGNALS_IN_SEC, pageDisplay, FuncActive, PageDisplay::OnChanged_RefreshFPS, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cScaleYtype,                                                                                          //--- ДИСПЛЕЙ - Смещение ---
    "Смещение", "Оffset",
    "Задаёт режим удержания смещения по вертикали\n1. \"Напряжение\" - сохраняется наряжение смещения.\n2. \"Деления\" - сохраняется положение "
    "смещения на экране.",
    "Sets the mode of retaining the vertical displacement\n1. \"Voltage\" - saved dressing bias.\n2. \"Divisions\" - retained the position of "
    "the offset on the screen.",
    "Напряжение", "Voltage",
    "Деления", "Divisions",
    LINKING_RSHIFT, pageDisplay, FuncActive, FuncChangedChoice, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_10( pageDisplay,                                                                                                            //--- ДИСПЛЕЙ ---
    "ДИСПЛЕЙ", "DISPLAY",
    "Содержит настройки отображения дисплея.",
    "Contains settings of display of the display.",
    &cViewMode,                             ///< ДИСПЛЕЙ - Отображение
    PageDisplay::PageAccumulation::pointer, ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ
    PageDisplay::PageAverage::pointer,      ///< ДИСПЛЕЙ - УСРЕДНЕНИЕ
    &cMinMax,                               ///< ДИСПЛЕЙ - Мин Макс
    &cSmoothing,                            ///< ДИСПЛЕЙ - Сглаживание
    &cRefreshFPS,                           ///< ДИСПЛЕЙ - Частота обновл
    PageDisplay::PageGrid::pointer,         ///< ДИСПЛЕЙ - СЕТКА
    &cScaleYtype,                           ///< ДИСПЛЕЙ - Смещение
    PageDisplay::PageSettings::pointer,     ///< ДИСПЛЕЙ - НАСТРОЙКИ
    &cThickness,                            ///< ДИСПЛЕЙ - Толщина
    Page::Name::Display, Menu::pageMain, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)
