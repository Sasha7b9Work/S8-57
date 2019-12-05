#include "defines.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Settings/Settings.h"


static int16 shiftADCA;
static int16 shiftADCB;

static void Draw_Balance_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, set.ch[Chan::A].balanceShiftADC, static_cast<int8>(set.dbg.nrst.balanceADC[Chan::A])},
        {0, set.ch[Chan::B].balanceShiftADC, static_cast<int8>(set.dbg.nrst.balanceADC[Chan::B])}
    };

    shiftADCA = shift[Chan::A][set.dbg.nrst.balanceADCtype];
    shiftADCB = shift[Chan::B][set.dbg.nrst.balanceADCtype];
}

static void OnChanged_Balance_Mode(bool)
{
    Draw_Balance_Mode(0, 0);
}

DEF_CHOICE_3(cBalance_Mode,                                                                                                                         //--- ОТЛАДКА - АЦП - БАЛАНС - Режим ---
    "Режим",
    "",
    DISABLE_RU,
    "Реальный",
    "Ручной",
    set.dbg.nrst.balanceADCtype, &PageDebug::PageADC::PageBalance::self, Item::Active, OnChanged_Balance_Mode, Draw_Balance_Mode
)


static bool IsActive_ShiftAB()
{
    return (set.dbg.nrst.balanceADCtype == BalanceADC::Hand);
}

static void OnChanged_ShiftA()
{
    set.dbg.nrst.balanceADC[Chan::A] = shiftADCA;
}

DEF_GOVERNOR(gShiftA,                                                                                                                          //--- ОТЛАДКА - АЦП - БАЛАНС - Смещение 1 ---
    "Смещение 1",
    "",
    shiftADCA, -125, 125,
    &PageDebug::PageADC::PageBalance::self, IsActive_ShiftAB, OnChanged_ShiftA
)


static void OnChanged_ShiftB()
{
    set.dbg.nrst.balanceADC[Chan::B] = shiftADCB;
}

DEF_GOVERNOR(gShiftB,                                                                                                                          //--- ОТЛАДКА - АЦП - БАЛАНС - Смещение 2 ---
    "Смещение 2",
    "",
    shiftADCB, -125, 125,
    &PageDebug::PageADC::PageBalance::self, IsActive_ShiftAB, OnChanged_ShiftB
)


DEF_PAGE_3(pBalance,                                                                                                                       //--- ОТЛАДКА - АЦП - БАЛАНС ---
    "БАЛАНС",
    "",
    &cBalance_Mode,
    &gShiftA,
    &gShiftB,
    PageName::Debug_ADC_Balance,
    &PageDebug::PageADC::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)


DEF_CHOICE_3(cStretch_Mode,                                                                                                                   //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Режим ---
    "Режим",
    "",
    DISABLE_RU,
    "Реальный",
    "Ручной",
    set.dbg.nrst.stretchADCtype, &PageDebug::PageADC::PageStretch::self, Item::Active, PageDebug::PageADC::PageStretch::OnChanged_Mode, Choice::AfterDraw
)


static int16 stretchA;
static int16 stretchB;


void PageDebug::PageADC::PageStretch::OnChanged_Mode(bool)
{
    if (set.dbg.nrst.stretchADCtype == StretchADC::Disable)
    {
        stretchA = set.dbg.nrst.stretchADC[Chan::A][StretchADC::Disable] = 0;
        stretchB = set.dbg.nrst.stretchADC[Chan::B][StretchADC::Disable] = 0;
    }
    else
    {
        stretchA = set.dbg.nrst.stretchADC[Chan::A][set.dbg.nrst.stretchADCtype];
        stretchB = set.dbg.nrst.stretchADC[Chan::B][set.dbg.nrst.stretchADCtype];
    }
}


static bool IsActive_StretchAB()
{
    return (set.dbg.nrst.stretchADCtype == StretchADC::Hand);
}

static void OnChanged_Stretch_A()
{
    set.dbg.nrst.stretchADC[Chan::A][set.dbg.nrst.stretchADCtype] = stretchA;
}

DEF_GOVERNOR(gStretch_A,                                                                                                                //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 1к ---
    "Растяжка 1к",
    "Задаёт ручную растяжку первого канала.\n1 единица = 0.0001",
    stretchA, -10000, 10000,
    &PageDebug::PageADC::PageStretch::self, IsActive_StretchAB, OnChanged_Stretch_A
)


static void OnChanged_Stretch_B()
{
    set.dbg.nrst.stretchADC[Chan::B][set.dbg.nrst.stretchADCtype] = stretchB;
}

DEF_GOVERNOR(gStretch_B,                                                                                                                //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 2к ---
    "Растяжка 2к",
    "Задаёт ручную растяжку второго канала.\n1 единица = 0.0001",
    stretchB, -10000, 10000,
    &PageDebug::PageADC::PageStretch::self, IsActive_StretchAB, OnChanged_Stretch_B
)


DEF_PAGE_3(pStretch,                                                                                                                     //--- ОТЛАДКА - АЦП - РАСТЯЖКА ---
    "РАСТЯЖКА",
    "Устанавливает режим и величину растяжки (для ручного режима)",
    &cStretch_Mode,
    &gStretch_A,
    &gStretch_B,
    PageName::Debug_ADC_Stretch,
    &PageDebug::PageADC::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)





DEF_PAGE_2(pADC,  //-V1027
    "АЦП",
    "",
    &pBalance,
    &pStretch,
    PageName::Debug_ADC,
    &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageDebug::PageADC::self = static_cast<const Page *>(&pADC);
const Page *const PageDebug::PageADC::PageStretch::self = static_cast<const Page *>(&pStretch);
const Page *const PageDebug::PageADC::PageBalance::self = static_cast<const Page *>(&pBalance);
