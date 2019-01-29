#include "defines.h"
#include "Display/Colors.h"
#include "Display/Painter.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageDebug.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageConsole;
extern const PageBase pageRegisters;

const PageBase *PageDebug::PageConsole::pointer = &pageConsole;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_GOVERNOR( gNumStrings,                                                                                                                          //--- ОТЛАДКА - КОНСОЛЬ - Число строк ---
    "Число строк", "Number strings",
    "",
    "",
    CONSOLE_NUM_STRINGS, 0, 33, pageConsole, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cSizeFont,                                                                                                                          //--- ОТЛАДКА - КОНСОЛЬ - Размер шрифта ---
    "Размер шрифта", "Size font",
    "",
    "",
    "5", "5",
    "8", "8",
    set.dbg_sizeFont, pageConsole, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cModeStop,                                                                                                                          //--- ОТЛАДКА - КОНСОЛЬ - Реж. останова ---
    "Реж. останова", "Mode stop",
    "Предоставляет возможность приостановки вывода в консоль путём нажатия на кнопку ПУСК/СТОП",
    "It provides the ability to pause the output to the console by pressing the ПУСК/СТОП button",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    MODE_PAUSE_CONSOLE, pageConsole, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_ShowAll,                                                                                                     //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Показывать все ---
    "Показывать все", "Show all",
    "Показывать все значения, засылаемые в регистры",
    "To show all values transferred in registers",
    "Нет", "No",
    "Да", "Yes",
    DBG_SHOW_ALL, pageRegisters, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Console_Registers()
{
    return DBG_SHOW_ALL;
}

DEF_CHOICE_2( cRegisters_RD_FL,                                                                                                                //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - RD_FL ---
    "RD_FL", "RD_FL",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    DBG_SHOW_FLAG, pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_RShiftA,                                                                                                           //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 1к ---
    "U см. 1к", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showRShift[Chan::A], pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_RShiftB,                                                                                                           //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 2к ---
    "U см. 2к", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showRShift[Chan::B], pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_TrigLev,                                                                                                           //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U синхр. ---
    "U синхр.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showTrigLev, pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_RangeA,                                                                                                         //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 1 ---
    "ВОЛЬТ/ДЕЛ 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showRange[Chan::A], pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_RangeB,                                                                                                         //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 2 ---
    "ВОЛЬТ/ДЕЛ 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showRange[Chan::B], pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_TrigParam,                                                                                                    //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. синхр. ---
    "Парам. синхр.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showTrigParam, pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_ChanParamA,                                                                                                   //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2 ---
    "Парам. кан. 1", "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showChanParam[Chan::A], pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_ChanParamB,                                                                                                   //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2 ---
    "Парам. кан. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showChanParam[Chan::B], pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_TBase,                                                                                                            //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВРЕМЯ/ДЕЛ ---
    "ВРЕМЯ/ДЕЛ", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showTBase, pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cRegisters_TShift,                                                                                                               //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Т см. ---
    "Т см.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showTShift, pageRegisters, IsActive_Console_Registers, Choice::EmptyChange, Choice::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_12( pageRegisters, // -V641                                                                                                                   //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ ---
    "РЕГИСТРЫ", "REGISTERS",
    "",
    "",
    &cRegisters_ShowAll,        ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Показывать все
    &cRegisters_RD_FL,          ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - RD_FL
    &cRegisters_RShiftA,        ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 1к
    &cRegisters_RShiftB,        ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 2к
    &cRegisters_TrigLev,        ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U синхр
    &cRegisters_RangeA,         ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 1
    &cRegisters_RangeB,         ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 2
    &cRegisters_TrigParam,      ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. синхр.
    &cRegisters_ChanParamA,     ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 1
    &cRegisters_ChanParamB,     ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2
    &cRegisters_TBase,          ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВРЕМЯ/ДЕЛ
    &cRegisters_TShift,         ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Т см.
    Page::Name::Debug_Console_Registers, &pageConsole, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Console_SizeSettings(int x, int y)
{
    String("Разм.настр. %d", sizeof(Settings)).Draw(x + 6, y + 13, Color::BACK);
}

DEF_BUTTON( bSizeSettings,                                                                                                                      //--- ОТЛАДКА - КОНСОЛЬ - Размер настроек ---
    "", "",
    "Показывает текущий размер структуры для сохранения настроек",
    "Displays the current size of the structure to save settings",
    pageConsole, FuncActive, FuncPress, Draw_Console_SizeSettings
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5( pageConsole, // -V641 // -V1027                                                                                                                       //--- ОТЛАДКА - КОНСОЛЬ ---
    "КОНСОЛЬ", "CONSOLE",
    "",
    "",
    &gNumStrings,       ///< ОТЛАДКА - КОНСОЛЬ - Число строк
    &cSizeFont,         ///< ОТЛАДКА - КОНСОЛЬ - Размер шрифта
    &cModeStop,         ///< ОТЛАДКА - КОНСОЛЬ - Реж. останова
    &pageRegisters,     ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ
    &bSizeSettings,     ///< ОТЛАДКА - КОНСОЛЬ - Размер настроек
    Page::Name::Debug_Console, PageDebug::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

