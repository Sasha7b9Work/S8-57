#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const Page pageColors;

const Page * const PageDisplay::PageSettings::PageColors::self = (const Page *)&pageColors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_ResetColors()
{
}

DEF_BUTTON( bReset,                                                                                                                          //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сбросить ---
    "Сбросить",
    "Сброс всех цветов на значения по умолчанию",
    &PageDisplay::PageSettings::PageColors::self, E_BtV, OnPress_ResetColors, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeA = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::Channel(Chan::A))

DEF_GOVERNOR_COLOR( gcChannelA,                                                                                                               //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1 ---
    "Канал 1",
    "Выбор цвета канала 1",
    PageDisplay::PageSettings::PageColors::colorTypeA, &PageDisplay::PageSettings::PageColors::self
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeB = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::Channel(Chan::B))

DEF_GOVERNOR_COLOR( gcChannelB,                                                                                                               //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2 ---
    "Канал 2",
    "Выбор цвета канала 1",
    PageDisplay::PageSettings::PageColors::colorTypeB, &PageDisplay::PageSettings::PageColors::self
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeGrid = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::GRID)

DEF_GOVERNOR_COLOR( gcGrid,                                                                                                                     //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка ---
    "Сетка",
    "Устанавливает цвет сетки",
    PageDisplay::PageSettings::PageColors::colorTypeGrid, &PageDisplay::PageSettings::PageColors::self
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_4( pageColors, // -V641 // -V1027                                                                                                              //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА ---
    "ЦВЕТА",
    "Выбор цветов дисплея",
    &bReset,              ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сбросить
    &gcChannelA,          ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1
    &gcChannelB,          ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2
    &gcGrid,              ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка
    PageName::Display_Settings_Colors, &PageDisplay::PageSettings::self, E_BtV, E_VB, E_VV, E_BfKE
)
