#include "defines.h"
#include "Menu/pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Brightness()
{
}

DEF_GOVERNOR( gBrightness,                                                                                                                            //--- ДИСПЛЕЙ - НАСТРОЙКИ - Яркость ---
    "Яркость",
    "Установка яркости свечения дисплея",
    BRIGHTNESS_DISPLAY, 0, 100, &PageDisplay::PageSettings::self, Item::Active, OnChanged_Brightness
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_AutoHide(bool autoHide)
{
    Menu::SetAutoHide(autoHide);
}

DEF_CHOICE_6( cAutoHide,                                                                                                                             //--- ДИСПЛЕЙ - НАСТРОЙКИ - Скрывать ---
    "Скрывать",
    "Установка после последнего нажатия кнопки или поворота ручки, по истечении которого меню автоматически убирается с экрана",
    "Никогда",
    "Через 5 сек",
    "Через 10 сек",
    "Через 15 сек",
    "Через 30 сек",
    "Через 60 сек",
    set.disp.menuAutoHide, &PageDisplay::PageSettings::self, Item::Active, OnChanged_AutoHide, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3( pSettings, // -V641 // -V1027                                                                                                                       //--- ДИСПЛЕЙ - НАСТРОЙКИ ---
    "НАСТРОЙКИ",
    "Дополнительные настройки дисплея",
    PageDisplay::PageSettings::PageColors::self,        ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА
    &gBrightness,                                       ///< ДИСПЛЕЙ - НАСТРОЙКИ - Яркость
    &cAutoHide,                                         ///< ДИСПЛЕЙ - НАСТРОЙКИ - Скрывать
    PageName::Display_Settings, &PageDisplay::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageDisplay::PageSettings::self = (const Page *)&pSettings;
