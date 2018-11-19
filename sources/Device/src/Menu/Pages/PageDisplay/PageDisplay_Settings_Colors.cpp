#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageColors;

const PageBase *PageDisplay::PageSettings::PageColors::pointer = &pageColors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_ResetColors()
{
    Settings::ResetColors();
}

DEF_BUTTON( bReset,                                                                                   //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сбросить ---
    "Сбросить", "Reset",
    "Сброс всех цветов на значения по умолчанию",
    "Reset all colors to default values",
    pageColors, EmptyFuncBV, OnPress_ResetColors, EmptyFuncVII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cScheme,                                                                          //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Цветовая схема ---
    "Цветовая схема", "Color scheme",
    "Изменение цветовой схемы",
    "Changing the color scheme",
    "Схема 1", "Scheme 1",
    "Схема 2", "Scheme 2",
    set.serv_colorScheme, pageColors, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeA = COLOR_TYPE(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::Channel(Chan::A))

DEF_GOVERNOR_COLOR( gcChannelA,                                                                        //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1 ---
    "Канал 1", "Chan 1",
    "Выбор цвета канала 1",
    "Choice of channel 1 color",
    PageDisplay::PageSettings::PageColors::colorTypeA, pageColors
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeB = COLOR_TYPE(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::Channel(Chan::B))

DEF_GOVERNOR_COLOR( gcChannelB,                                                                        //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2 ---
    "Канал 2", "Chan 2",
    "Выбор цвета канала 1",
    "Choice of channel 2 color",
    PageDisplay::PageSettings::PageColors::colorTypeB, pageColors
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeGrid = COLOR_TYPE(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, Color::GRID)
DEF_GOVERNOR_COLOR( gcGrid,                                                                              //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка ---
    "Сетка", "Grid",
    "Устанавливает цвет сетки",
    "Sets the grid color",
    PageDisplay::PageSettings::PageColors::colorTypeGrid, pageColors
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageDisplay::PageSettings::PageColors::OnChanged_Settings_Colors_Background(bool)
{
    Color::InitGlobalColors();

    gcChannelA.ct->color = Color::Channel(Chan::A);
    gcChannelB.ct->color = Color::Channel(Chan::B);
    gcGrid.ct->color = Color::GRID;

    gcChannelA.ct->Init(true);
    gcChannelB.ct->Init(true);
    gcGrid.ct->Init(true);
}

DEF_CHOICE_2( cBackground,                                                                                 //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Фон ---
    "Фон", "Background",
    "Выбор цвета фона",
    "Choice of color of a background",
    "Чёрный", "Black",
    "Белый", "White",
    BACKGROUND, pageColors, FuncActive, PageDisplay::PageSettings::PageColors::OnChanged_Settings_Colors_Background, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \todo Добавить дополнительные цвета 1-го и 2-го каналов
DEF_PAGE_6( pageColors,                                                                                          //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА ---
    "ЦВЕТА", "COLORS",
    "Выбор цветов дисплея",
    "The choice of colors display",
    &bReset,              ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сбросить
    &cScheme,             ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Цветовая схема
    &gcChannelA,          ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1
    &gcChannelB,          ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2
    &gcGrid,              ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка
    &cBackground,         ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Фон
    Page::Name::Display_Settings_Colors, PageDisplay::PageSettings::pointer, FuncActive, EmptyPressPage
)
