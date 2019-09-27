#include "defines.h"
#include "Hardware/Battery.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cShow,                                                                                                                                  //--- СЕРВИС - БАТАРЕЯ - Показывать ---  
    "Показывать",
    "Включает/отключает вывод иноформации о напряжении батарии на экран",
    DISABLE_RU,
    ENABLE_RU,
    BAT_SHOW_ON_DISPLAY, &PageService::PageBattery::self, Item::EmptyActive, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_1( pBattery, // -V641 // -V1027                                                                                                                           //--- СЕРВИС - БАТАРЕЯ ---
    "БАТАРЕЯ",
    "Функции работы с аккумуляторами",
    &cShow,                     ///< СЕРВИС - БАТАРЕЯ - Показывать
    PageName::Service_Battery, &PageService::self, Item::EmptyActive, E_VB, Page::EmptyBeforeDraw, E_BfKE
)

const Page * const PageService::PageBattery::self = (const Page *)&pBattery;
