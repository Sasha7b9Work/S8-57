#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Hardware/Battery.h"
#include "Menu/Pages/Include/PageService.h"
#endif

extern const PageBase pageBattery;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageService::PageBattery::pointer = &pageBattery;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cShow,                                                                                                                                  //--- СЕРВИС - БАТАРЕЯ - Показывать ---  
    "Показывать", "Show",
    "Включает/отключает вывод иноформации о напряжении батарии на экран",
    "Enables / disables the display of battery voltage information on the screen",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    BAT_SHOW_ON_DISPLAY, pageBattery, FuncActive, Choice::EmptyChange, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_1( pageBattery, // -V641 // -V1027                                                                                                                        //--- СЕРВИС - БАТАРЕЯ ---
    "БАТАРЕЯ", "BATTERY",
    "Функции работы с аккумуляторами",
    "Battery functions",
    &cShow,                     ///< СЕРВИС - БАТАРЕЯ - Показывать
    Page::Name::Service_Battery, PageService::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
