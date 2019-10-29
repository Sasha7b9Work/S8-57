#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"



static void OnChanged_HoldOff(bool)
{
    Osci::LoadHoldfOff();
}

DEF_CHOICE_2( cHoldOff,                                                                                                                               //--- СИНХР - УДЕРЖАНИЕ - Удержание ---
    "Удержание",
    "Включает/отключает режим задержки синхронизации"
    ,
    DISABLE_RU,
    ENABLE_RU,
    set.trig.holdOffEnabled, &PageTrig::PageHoldOff::self, Item::Active, OnChanged_HoldOff, Choice::AfterDraw
)



static void OnChanged_Duration()
{
    Osci::LoadHoldfOff();
}

DEF_GOVERNOR( gDuration,                                                                                                                           //--- СИНХР - УДЕРЖАНИЕ - Длительность ---
    "Длительность",
    "Задаёт длительность промежутка между импульсами синхронизации",
    set.trig.holdOff, 10, 32000, &PageTrig::PageHoldOff::self, Item::Active, OnChanged_Duration
)

/// \todo gDuration должен обеспечивать возможность хранения большего значения, чем 32000



DEF_PAGE_2( pHoldOff,                                                                                                                                             //--- СИНХР - УДЕРЖАНИЕ ---
    "УДЕРЖАНИЕ",
    "",
    &cHoldOff,
    &gDuration,
    PageName::Trig_HoldOff,
    &PageTrig::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageTrig::PageHoldOff::self = static_cast<const Page *>(&pHoldOff);
