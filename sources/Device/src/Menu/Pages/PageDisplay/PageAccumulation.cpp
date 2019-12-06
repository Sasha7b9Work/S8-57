#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"


ModeAccumulation::E &ModeAccumulation::Ref()
{
    return set.disp.modeAccumulation;
}


DEF_CHOICE_2( cEnable,                                                                                                                                 //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим ---
    "Режим",
    "",
    DISABLE_RU,
    ENABLE_RU,
    ModeAccumulation::Ref(), &PageDisplay::PageAccumulation::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



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
    ENumAccum::Ref(), &PageDisplay::PageAccumulation::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



static bool IsActive_Accum_Clear()
{
    return true;
}

void PageDisplay::PageAccumulation::OnPress_Accumulation_Clear()
{
    DisplayOsci::SetFlagRedraw();
}

DEF_BUTTON( bClear,                                                                                                                                 //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить ---
    "Очистить",
    "Очищает экран от накопленных сигналов.",
    &PageDisplay::PageAccumulation::self, IsActive_Accum_Clear, PageDisplay::PageAccumulation::OnPress_Accumulation_Clear
)



static bool IsActive_Accum()
{
    return !Osci::InModeRandomizer();
}

DEF_PAGE_3( pAccum,                                                                                                                           //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ ---
    "НАКОПЛ",
    "Настройки режима отображения последних сигналов на экране",
    &cEnable,
    &cNumber,
    &bClear,
    PageName::Display_Accumulation, &PageDisplay::self, IsActive_Accum, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDisplay::PageAccumulation::self = static_cast<const Page *>(&pAccum);
