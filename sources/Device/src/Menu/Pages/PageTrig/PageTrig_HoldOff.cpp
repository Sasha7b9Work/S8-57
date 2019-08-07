#include "defines.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"


extern const PageBase pageHoldOff;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageTrig::PageHoldOff::pointer = &pageHoldOff;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_HoldOff(bool)
{
    Osci::Settings::LoadHoldfOff();
}

DEF_CHOICE_2( cHoldOff,                                                                                                                               //--- СИНХР - УДЕРЖАНИЕ - Удержание ---
    "Удержание",
    "Включает/отключает режим задержки синхронизации"
    ,
    DISABLE_RU,
    ENABLE_RU,
    TRIG_HOLDOFF_ENABLED, pageHoldOff, Choice::FuncActive, OnChanged_HoldOff, Choice::FuncDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Duration()
{
    Osci::Settings::LoadHoldfOff();
}

DEF_GOVERNOR( gDuration,                                                                                                                           //--- СИНХР - УДЕРЖАНИЕ - Длительность ---
    "Длительность",
    "Задаёт длительность промежутка между импульсами синхронизации",
    TRIG_HOLDOFF, 10, 32000, pageHoldOff, Governor::FuncActive, OnChanged_Duration, 0
)

/// \todo gDuration должен обеспечивать возможность хранения большего значения, чем 32000


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_2( pageHoldOff, // -V641 // -V1027                                                                                                                       //--- СИНХР - УДЕРЖАНИЕ ---
    "УДЕРЖАНИЕ",
    "",
    &cHoldOff,          ///< СИНХР - УДЕРЖАНИЕ - Удержание
    &gDuration,         ///< СИНХР - УДЕРЖАНИЕ - Длительность
    Page::Name::Trig_HoldOff, PageTrig::pointer, Page::FuncActive, Page::FuncPress, Page::FuncDraw, FuncRegSetPage
)
