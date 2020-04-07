#include "defines.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Menu/Pages/Include/PageService.h"
#include "Osci/Osci.h"
#include "Settings/SettingsNRST.h"


static void OnChanged_Gates(bool)
{
    Osci::ClearDataRand();
}


DEF_CHOICE_21( gGateMax,                                                                                                                           //--- ОТЛАДКА - РАНД-ТОР - Ворота верх ---
    "Ворота верх",
    "Устанавливает учитываемое расстояние от максимума ворот для рандомизатора",
    "0",
    "10",
    "20",
    "30",
    "40",
    "50",
    "60",
    "70",
    "80",
    "90",
    "100",
    "110",
    "120",
    "130",
    "140",
    "150",
    "160",
    "170",
    "180",
    "190",
    "200",
    setNRST.enumGameMax, &PageDebug::PageRand::self, Item::Active, OnChanged_Gates, Choice::AfterDraw
)



DEF_CHOICE_21( gGateMin,                                                                                                                            //--- ОТЛАДКА - РАНД-ТОР - Ворота низ ---
    "Ворота низ",
    "Устанавливает учитываемое расстояние от минимума ворот для рандомизатора",
    "0",
    "10",
    "20",
    "30",
    "40",
    "50",
    "60",
    "70",
    "80",
    "90",
    "100",
    "110",
    "120",
    "130",
    "140",
    "150",
    "160",
    "170",
    "180",
    "190",
    "200",
    setNRST.enumGameMin, &PageDebug::PageRand::self, Item::Active, OnChanged_Gates, Choice::AfterDraw
)


DEF_PAGE_2( pRand,                                                                                                                                               //--- ОТЛАДКА - РАНД-ТОР ---
    "РАНД-ТОР",
    "",
    &gGateMin,
    &gGateMax,
    PageName::Debug_Rand, &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageDebug::PageRand::self = static_cast<const Page *>(&pRand);
