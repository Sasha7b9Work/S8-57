#include "defines.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Settings/SettingsNRST.h"


static int16 shift1 = setNRST._exShift[0][0];
static int16 shift2 = setNRST._exShift[1][0];


static void OnChange_Shift1()
{
    setNRST.SetExtraShift(static_cast<int8>(shift1));
}

DEF_GOVERNOR( gExShift1,
    "Баланс 1",
    "",
    shift1, -100, 100, &PageDebug::PageADC::self, Item::Active, OnChange_Shift1, Governor::AfterDraw
)


static void OnChange_Shift2()
{
    setNRST.SetExtraShift(static_cast<int8>(shift2));
}


DEF_GOVERNOR( gExShift2,
    "Баланс 2",
    "",
    shift2, -100, 100, &PageDebug::PageADC::self, Item::Active, OnChange_Shift2, Governor::AfterDraw
)


DEF_PAGE_2( pADC, //-V2567
    "АЦП",
    "",
    &gExShift1,
    &gExShift2,
    PageName::Debug_ADC, &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageADC::self = static_cast<const Page *>(&pADC);
