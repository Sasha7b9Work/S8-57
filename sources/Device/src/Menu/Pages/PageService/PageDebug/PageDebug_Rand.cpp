#include "defines.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/SettingsNRST.h"


DEF_CHOICE_16( gGateMax,                                                                                                                           //--- ОТЛАДКА - РАНД-ТОР - Ворота верх ---
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
    setNRST.enum_gate_max, &PageDebug::PageRand::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



DEF_CHOICE_16( gGateMin,                                                                                                                            //--- ОТЛАДКА - РАНД-ТОР - Ворота низ ---
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
    setNRST.enum_gate_min, &PageDebug::PageRand::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



DEF_PAGE_2( pRand,                                                                                                                                               //--- ОТЛАДКА - РАНД-ТОР ---
    "РАНД-ТОР",
    "",
    &gGateMin,
    &gGateMax,
    PageName::Debug_Rand, &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageDebug::PageRand::self = static_cast<const Page *>(&pRand);
