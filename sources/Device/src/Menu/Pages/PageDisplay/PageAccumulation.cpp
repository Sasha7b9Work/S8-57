#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Osci/Osci.h"
#include "Osci/Display/Osci_Display.h"
#include "Settings/Settings.h"


using namespace Osci::Settings;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cEnable,                                                                                                                                 //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим ---
    "Режим",
    "",
    DISABLE_RU,
    ENABLE_RU,
    ACCUMULATION, &PageDisplay::PageAccumulation::self, Item::EmptyActive, E_VB, E_VII
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_9( cNumber,                                                                                                                            //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Количество ---
    "Количество"
    ,
    "Задаёт максимальное количество последних сигналов на экране. Если в настройке \"Режим\" выбрано \"Бесконечность\", экран очищается только нажатием кнопки \"Очистить\"."
    "\"Бесконечность\" - каждое измерение остаётся на дисплее до тех пор, пока не будет нажата кнопка \"Очистить\".",
    DISABLE_RU,
    "2",
    "4",
    "8",
    "16",
    "32",
    "64",
    "128",
    "Бесконечность",
    ENUM_ACCUM, &PageDisplay::PageAccumulation::self, Item::EmptyActive, E_VB, E_VII
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Accum_Clear()
{
    return true;
}

void PageDisplay::PageAccumulation::OnPress_Accumulation_Clear()
{
    Osci::Display::SetFlagRedraw();
}

DEF_BUTTON( bClear,                                                                                                                                 //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить ---
    "Очистить",
    "Очищает экран от накопленных сигналов.",
    &PageDisplay::PageAccumulation::self, IsActive_Accum_Clear, PageDisplay::PageAccumulation::OnPress_Accumulation_Clear
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Accum()
{
    return !Osci::InModeRandomizer();
}

DEF_PAGE_3( pAccum, // -V641 // -V1027                                                                                                                         //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ ---
    "НАКОПЛ",
    "Настройки режима отображения последних сигналов на экране",
    &cEnable,    ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ - Количество
    &cNumber,    ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим
    &bClear,     ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить
    PageName::Display_Accumulation, &PageDisplay::self, IsActive_Accum, E_VB, Page::EmptyBeforeDraw, E_BfKE
)

const Page * const PageDisplay::PageAccumulation::self = (const Page *)&pAccum;
