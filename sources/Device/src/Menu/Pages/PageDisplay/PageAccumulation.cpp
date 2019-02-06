#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"


using namespace Osci::Settings;


extern const PageBase pageAccum;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageDisplay::PageAccumulation::pointer = &pageAccum;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cEnable,                                                                                                                                 //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим ---
    "Режим", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    ACCUMULATION, pageAccum, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_9( cNumber,                                                                                                                            //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Количество ---
    "Количество", "Number"
    ,
    "Задаёт максимальное количество последних сигналов на экране. Если в настройке \"Режим\" выбрано \"Бесконечность\", экран очищается только "
    "нажатием кнопки \"Очистить\"."
    "\"Бесконечность\" - каждое измерение остаётся на дисплее до тех пор, пока не будет нажата кнопка \"Очистить\"."
    ,
    "Sets the maximum quantity of the last signals on the screen. If in control \"Mode\" it is chosen \"Infinity\", the screen is cleared only "
    "by pressing of the button \"Clear\"."
    "\"Infinity\" - each measurement remains on the display until the button \"Clear\" is pressed.",
    DISABLE_RU, DISABLE_EN,
    "2", "2",
    "4", "4",
    "8", "8",
    "16", "16",
    "32", "32",
    "64", "64",
    "128", "128",
    "Бесконечность", "Infinity",
    ENUM_ACCUM, pageAccum, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Accum_Clear()
{
    return true;
}

void PageDisplay::PageAccumulation::OnPress_Accumulation_Clear()
{
    NEED_FINISH_DRAW = 1;
}

DEF_BUTTON( bClear,                                                                                                                                 //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить ---
    "Очистить", "Clear",
    "Очищает экран от накопленных сигналов.",
    "Clears the screen of the saved-up signals.",
    pageAccum, IsActive_Accum_Clear, PageDisplay::PageAccumulation::OnPress_Accumulation_Clear, Button::FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Accum()
{
    return !Osci::InModeRandomizer();
}

DEF_PAGE_3(pageAccum, // -V641 // -V1027                                                                                                                       //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ ---
    "НАКОПЛ", "ACCUM",
    "Настройки режима отображения последних сигналов на экране",
    "Mode setting signals to display the last screen",
    &cEnable,    ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ - Количество
    &cNumber,    ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим
    &bClear,     ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить
    Page::Name::Display_Accumulation, PageDisplay::pointer, IsActive_Accum, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
