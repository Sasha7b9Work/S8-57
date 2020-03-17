#include "defines.h"
#include "Menu/Pages/Include/DebugPage.h"
//#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


ShiftADC &ShiftADC::Ref()
{
    return setNRST.shiftADC;
}


StretchADC &StretchADC::Ref()
{
    return setNRST.stretchADC;
}


static int16 shiftADCA;
static int16 shiftADCB;


static void Draw_Balance_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, setNRST.balanceShiftADC[Chan::A], static_cast<int8>(BalanceADC::Value(Chan::A))},
        {0, setNRST.balanceShiftADC[Chan::B], static_cast<int8>(BalanceADC::Value(Chan::B))}
    };

    shiftADCA = shift[Chan::A][BalanceADC::Ref().value];
    shiftADCB = shift[Chan::B][BalanceADC::Ref().value];
}

static void OnChanged_Balance_Mode(bool)
{
    Draw_Balance_Mode(0, 0);
}

DEF_CHOICE_3(cBalance_Mode,                                                                                                                          //--- ОТЛАДКА - АЦП - БАЛАНС - Режим ---
    "Режим",
    "",
    DISABLE_RU,
    "Реальный",
    "Ручной",
    BalanceADC::Ref(), &PageDebug::PageADC::PageBalance::self, Item::Active, OnChanged_Balance_Mode, Draw_Balance_Mode
)


static bool IsActive_ShiftAB()
{
    return BalanceADC::IsHand();
}

static void OnChanged_ShiftA()
{
    BalanceADC::Value(Chan::A) = shiftADCA;
}

DEF_GOVERNOR(gShiftA,                                                                                                                           //--- ОТЛАДКА - АЦП - БАЛАНС - Смещение 1 ---
    "Смещение 1",
    "",
    shiftADCA, -125, 125,
    &PageDebug::PageADC::PageBalance::self, IsActive_ShiftAB, OnChanged_ShiftA
)


static void OnChanged_ShiftB()
{
    BalanceADC::Value(Chan::B) = shiftADCB;
}

DEF_GOVERNOR(gShiftB,                                                                                                                           //--- ОТЛАДКА - АЦП - БАЛАНС - Смещение 2 ---
    "Смещение 2",
    "",
    shiftADCB, -125, 125,
    &PageDebug::PageADC::PageBalance::self, IsActive_ShiftAB, OnChanged_ShiftB
)


DEF_PAGE_3(pBalance,                                                                                                                                         //--- ОТЛАДКА - АЦП - БАЛАНС ---
    "БАЛАНС",
    "",
    &cBalance_Mode,
    &gShiftA,
    &gShiftB,
    PageName::Debug_ADC_Balance,
    &PageDebug::PageADC::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)


DEF_CHOICE_2(cStretch_Mode,                                                                                                                        //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Режим ---
    "Режим",
    "",
    DISABLE_RU,
    "Реальный",
    StretchADC::Ref(), &PageDebug::PageADC::PageStretch::self, Item::Active, PageDebug::PageADC::PageStretch::OnChanged_Mode, Choice::AfterDraw
)


static int16 stretchA;
static int16 stretchB;


void PageDebug::PageADC::PageStretch::OnChanged_Mode(bool)
{
    if (StretchADC::TypeIsDisabled())
    {
    }
    else
    {
        stretchA = static_cast<int16>(StretchADC::Value(Chan::A));
        stretchB = static_cast<int16>(StretchADC::Value(Chan::B));
    }
}


static bool IsActive_StretchAB()
{
    return StretchADC::TypeIsHand();
}

static void OnChanged_Stretch_A()
{
    //set.dbg.nrst.stretchADC[Chan::A] = stretchA;
}

DEF_GOVERNOR(gStretch_A,                                                                                                                     //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 1к ---
    "Растяжка 1к",
    "Задаёт ручную растяжку первого канала.\n1 единица = 0.0001",
    stretchA, -10000, 10000,
    &PageDebug::PageADC::PageStretch::self, IsActive_StretchAB, OnChanged_Stretch_A
)


static void OnChanged_Stretch_B()
{
    //set.dbg.nrst.stretchADC[Chan::B][set.dbg.nrst.stretchADCtype] = stretchB;
}

DEF_GOVERNOR(gStretch_B,                                                                                                                     //--- ОТЛАДКА - АЦП - РАСТЯЖКА - Растяжка 2к ---
    "Растяжка 2к",
    "Задаёт ручную растяжку второго канала.\n1 единица = 0.0001",
    stretchB, -10000, 10000,
    &PageDebug::PageADC::PageStretch::self, IsActive_StretchAB, OnChanged_Stretch_B
)


DEF_PAGE_3(pStretch,                                                                                                                                       //--- ОТЛАДКА - АЦП - РАСТЯЖКА ---
    "РАСТЯЖКА",
    "Устанавливает режим и величину растяжки (для ручного режима)",
    &cStretch_Mode,
    &gStretch_A,
    &gStretch_B,
    PageName::Debug_ADC_Stretch,
    &PageDebug::PageADC::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)


static void OnChanged_ShiftType(bool)
{
    RShift(Chan::A).Load(true);
    RShift(Chan::B).Load(true);
}


DEF_CHOICE_2(cAddRShift,
    "Режим",
    "",
    DISABLE_RU,
    "Реальный",
    ShiftADC::Ref(), &PageDebug::PageADC::PageShift::self, Item::Active, OnChanged_ShiftType, Choice::AfterDraw
)


DEF_PAGE_1(pShift,
    "ДОП. СМ.",
    "Управляет режимом дополнительного смщения по вертикали",
    &cAddRShift,
    PageName::Debug_ADC_Shift,
    &PageDebug::PageADC::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)


DEF_PAGE_3(pADC,  //-V1027
    "АЦП",
    "",
    &pBalance,
    &pStretch,
    &pShift,
    PageName::Debug_ADC,
    &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageDebug::PageADC::self = static_cast<const Page *>(&pADC);
const Page *const PageDebug::PageADC::PageStretch::self = static_cast<const Page *>(&pStretch);
const Page *const PageDebug::PageADC::PageBalance::self = static_cast<const Page *>(&pBalance);
const Page *const PageDebug::PageADC::PageShift::self = static_cast<const Page *>(&pShift);
