#include "defines.h"
#include "Menu/pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"


extern const PageDef pageSettings;

const Page * const PageDisplay::PageSettings::self = (const Page *)&pageSettings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_Settings_Brightness()
{
}

DEF_GOVERNOR( gBrightness,                                                                                                                            //--- ДИСПЛЕЙ - НАСТРОЙКИ - Яркость ---
    "Яркость",
    "Установка яркости свечения дисплея",
    BRIGHTNESS_DISPLAY, 0, 100, &PageDisplay::PageSettings::self, 0, OnChanged_Settings_Brightness, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_AutoHide(bool autoHide)
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
    MENU_AUTO_HIDE, &PageDisplay::PageSettings::self, 0, OnChanged_Settings_AutoHide, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pageSettings, // -V641 // -V1027                                                                                                                    //--- ДИСПЛЕЙ - НАСТРОЙКИ ---
    "НАСТРОЙКИ",
    "Дополнительные настройки дисплея",
    PageDisplay::PageSettings::PageColors::self,        ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА
    &gBrightness,                                       ///< ДИСПЛЕЙ - НАСТРОЙКИ - Яркость
    &cAutoHide,                                         ///< ДИСПЛЕЙ - НАСТРОЙКИ - Скрывать
    Page::Name::Display_Settings, &PageDisplay::self, 0, 0, 0, 0
)
