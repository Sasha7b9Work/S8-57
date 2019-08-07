#include "defines.h"
#include "Hardware/Battery.h"
#include "Menu/Pages/Include/PageService.h"

#include "Settings/Settings.h"


extern const PageBase pageBattery;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageService::PageBattery::pointer = &pageBattery;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cShow,                                                                                                                                  //--- СЕРВИС - БАТАРЕЯ - Показывать ---  
    "Показывать",
    "Включает/отключает вывод иноформации о напряжении батарии на экран",
    DISABLE_RU,
    ENABLE_RU,
    BAT_SHOW_ON_DISPLAY, pageBattery, FuncActive, Choice::FuncChange, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_1( pageBattery, // -V641 // -V1027                                                                                                                        //--- СЕРВИС - БАТАРЕЯ ---
    "БАТАРЕЯ",
    "Функции работы с аккумуляторами",
    &cShow,                     ///< СЕРВИС - БАТАРЕЯ - Показывать
    Page::Name::Service_Battery, PageService::pointer, FuncActive, FuncPressPage, Page::FuncDraw, FuncRegSetPage
)
