#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageSettings;

const PageBase *PageDisplay::PageSettings::pointer = &pageSettings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_Settings_Brightness()
{
}

DEF_GOVERNOR( gBrightness,                                                                                     //--- ДИСПЛЕЙ - НАСТРОЙКИ - Яркость ---
    "Яркость", "Brightness",
    "Установка яркости свечения дисплея",
    "Setting the brightness of the display",
    BRIGHTNESS_DISPLAY, 0, 100, pageSettings, FuncActive, OnChanged_Settings_Brightness, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gLevels,                                                                                          //--- ДИСПЛЕЙ - НАСТРОЙКИ - Уровни ---
    "Уровни", "Levels",
    "Задаёт время, в течение которого после поворота ручки сещения напряжения на экране остаётся вспомогательная метка уровня смещения",
    "Defines the time during which, after turning the handle visits to the voltage on the screen remains auxiliary label offset level",
    TIME_SHOW_LEVELS, 0, 125, pageSettings, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gTime,                                                                                             //--- ДИСПЛЕЙ - НАСТРОЙКИ - Время ---
    "Время", "Time",
    "Установка времени, в течение которого сообщения будут находиться на экране",
    "Set the time during which the message will be on the screen",
    TIME_MESSAGES, 1, 99, pageSettings, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_3( cStringNavigation,                                                                           //--- ДИСПЛЕЙ - НАСТРОЙКИ - Строка меню ---
    "Строка меню", "Path menu",
    "При выборе \nПоказывать\n слева вверху экрана выводится полный путь до текущей страницы меню", /// \todo Исправить перевод
    "When choosing \nDisplay\n at the top left of the screen displays the full path to the current page menu",
    "Временно", "Temporary",    /// \todo Исправить перевод
    "Всегда", "All",
    "Никогда", "None",
    SHOW_STRING_NAVI, pageSettings, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_AltMarkers(bool active)
{
    Display::ChangedRShiftMarkers(active);
}

DEF_CHOICE_3( cAltMarkers,                                                                                //--- ДИСПЛЕЙ - НАСТРОЙКИ - Доп. маркеры ---
    "Доп. маркеры", "Alt. markers",
    "Устанавливает режим отображения дополнительных маркеров уровней смещения и синхронизации:\n"
    "\"Скрывать\" - дополнительные маркеры не показываются,\n"
    "\"Показывать\" - дополнительные маркеры показываются всегда,\n"
    "\"Авто\" - дополнительные маркеры показываются в течение 5 сек после поворота ручки смещения канала по напряжению или уровня синхронизации",
    "Sets the display mode of additional markers levels of displacement and synchronization:\n"
    "\"Hide\" - additional markers are not shown,\n"
    "\"Show\" - additional markers are shown always,\n"
    "\"Auto\" - additional markers are displayed for 5 seconds after turning the handle channel offset voltage or trigger level"
    ,
    "Скрывать", "Hide",
    "Показывать", "Show",
    "Авто", "Auto",
    ALT_MARKERS, pageSettings, FuncActive, OnChanged_Settings_AltMarkers, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_AutoHide(bool autoHide)
{
    Menu::SetAutoHide(autoHide);
}

DEF_CHOICE_6( cAutoHide,                                                                                                                             //--- ДИСПЛЕЙ - НАСТРОЙКИ - Скрывать ---
    "Скрывать", "Hide",
    "Установка после последнего нажатия кнопки или поворота ручки, по истечении которого меню автоматически убирается с экрана",
    "Installation after the last keystroke or turning the handle, after which the menu will automatically disappear",
    "Никогда", "Never",
    "Через 5 сек", "Through 5 s",
    "Через 10 сек", "Through 10 s",
    "Через 15 сек", "Through 15 s",
    "Через 30 сек", "Through 30 s",
    "Через 60 сек", "Through 60 s",
    MENU_AUTO_HIDE, pageSettings, FuncActive, OnChanged_Settings_AutoHide, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_7( pageSettings, // -V641 // -V1027                                                                                                                    //--- ДИСПЛЕЙ - НАСТРОЙКИ ---
    "НАСТРОЙКИ", "SETTINGS",
    "Дополнительные настройки дисплея",
    "Additional display settings",
    PageDisplay::PageSettings::PageColors::pointer,     ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА
    &gBrightness,                                       ///< ДИСПЛЕЙ - НАСТРОЙКИ - Яркость
    &gLevels,                                           ///< ДИСПЛЕЙ - НАСТРОЙКИ - Уровни
    &gTime,                                             ///< ДИСПЛЕЙ - НАСТРОЙКИ - Время
    &cStringNavigation,                                 ///< ДИСПЛЕЙ - НАСТРОЙКИ - Строка меню
    &cAltMarkers,                                       ///< ДИСПЛЕЙ - НАСТРОЙКИ - Доп. маркеры
    &cAutoHide,                                         ///< ДИСПЛЕЙ - НАСТРОЙКИ - Скрывать
    Page::Name::Display_Settings, PageDisplay::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
