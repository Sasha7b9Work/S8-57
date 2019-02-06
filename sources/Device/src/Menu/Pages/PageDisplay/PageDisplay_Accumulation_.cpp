#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Settings/Settings.h"


using namespace Osci::Settings;


extern const PageBase pageAccum;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageDisplay::PageAccumulation::pointer = &pageAccum;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_REG_9( cAccum_Num,                                                                                                                     //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Количество ---
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
    "2",        "2",
    "4",        "4",
    "8",        "8",
    "16",       "16",
    "32",       "32",
    "64",       "64",
    "128",      "128",
    "Бесконечность", "Infinity",
    ENUM_ACCUM, pageAccum, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cAccum_Mode,                                                                                                                             //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим ---
    "Режим", "Mode"
    ,
    "1. \"Сбрасывать\" - после накопления заданного количества измерения происходит очистка дисплея. Этот режим удобен, когда памяти не хватает "
    "для сохранения нужного количества измерений.\n"
    "2. \"Не сбрасывать\" - на дисплей всегда выводится заданное или меньшее (в случае нехватки памяти) количество измерений. Недостатком является "
    "меньшее быстродействие и невозможность накопления заданного количества измерений при недостатке памяти."
    ,
    "1. \"Dump\" - after accumulation of the set number of measurement there is a cleaning of the display. This mode is convenient when memory "
    "isn't enough for preservation of the necessary number of measurements.\n"
    "2. \"Not to dump\" - the number of measurements is always output to the display set or smaller (in case of shortage of memory). Shortcoming "
    "is smaller speed and impossibility of accumulation of the set number of measurements at a lack of memory."
    ,
    "Не сбрасывать", "Not to dump",
    "Сбрасывать", "Dump",
    MODE_ACCUM, pageAccum, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Accum_Clear()
{
    return ENUM_ACCUM != Display::ENumAccum::_1 && !MODE_ACCUM_NO_RESET;
}

void PageDisplay::PageAccumulation::OnPress_Accumulation_Clear()
{
    NEED_FINISH_DRAW = 1;
}

DEF_BUTTON( bAccum_Clear,                                                                                                                           //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить ---
    "Очистить", "Clear",
    "Очищает экран от накопленных сигналов.",
    "Clears the screen of the saved-up signals.",
    pageAccum, IsActive_Accum_Clear, PageDisplay::PageAccumulation::OnPress_Accumulation_Clear, Button::FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Accum()
{
    return SET_TBASE > TBase::_20ns;
}

DEF_PAGE_3(pageAccum, // -V641 // -V1027                                                                                                                       //--- ДИСПЛЕЙ - НАКОПЛЕНИЕ ---
    "НАКОПЛ", "ACCUM",
    "Настройки режима отображения последних сигналов на экране",
    "Mode setting signals to display the last screen",
    &cAccum_Num,                          ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ - Количество
    &cAccum_Mode,                         ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ - Режим
    &bAccum_Clear,                        ///< ДИСПЛЕЙ - НАКОПЛЕНИЕ - Очистить
    Page::Name::Display_Accumulation, PageDisplay::pointer, IsActive_Accum, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
