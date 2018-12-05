#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Display/Colors.h"
#include "Display/Painter.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageDebug.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageConsole;
extern const PageBase pppConsole_Registers;

const PageBase *PageDebug::PageConsole::pointer = &pageConsole;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_GOVERNOR(gConsole_NumStrings,                                                                                                                   //--- ОТЛАДКА - КОНСОЛЬ - Число строк ---
    "Число строк", "Number strings",
    "",
    "",
    CONSOLE_NUM_STRINGS, 0, 33, pageConsole, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_SizeFont,                                                                                                                   //--- ОТЛАДКА - КОНСОЛЬ - Размер шрифта ---
    "Размер шрифта", "Size font",
    "",
    "",
    "5", "5",
    "8", "8",
    set.dbg_sizeFont, pageConsole, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_ModeStop,                                                                                                                   //--- ОТЛАДКА - КОНСОЛЬ - Реж. останова ---
    "Реж. останова", "Mode stop",
    "Предоставляет возможность приостановки вывода в консоль путём нажатия на кнопку ПУСК/СТОП",
    "It provides the ability to pause the output to the console by pressing the ПУСК/СТОП button",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    MODE_PAUSE_CONSOLE, pageConsole, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_ShowAll,                                                                                              //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Показывать все ---
    "Показывать все", "Show all",
    "Показывать все значения, засылаемые в регистры",
    "To show all values transferred in registers",
    "Нет", "No",
    "Да", "Yes",
    DBG_SHOW_ALL, pppConsole_Registers, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_Console_Registers()
{
    return DBG_SHOW_ALL;
}

DEF_CHOICE_2(cConsole_Registers_RD_FL,                                                                                                         //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - RD_FL ---
    "RD_FL", "RD_FL",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    DBG_SHOW_FLAG, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_RShiftA,                                                                                                    //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 1к ---
    "U см. 1к", "U shift 1ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showRShift[Chan::A], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_RShiftB,                                                                                                    //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 2к ---
    "U см. 2к", "U shift 2ch",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showRShift[Chan::B], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_TrigLev,                                                                                                    //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U синхр. ---
    "U синхр.", "U trig.",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showTrigLev, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_RangeA,                                                                                                  //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 1 ---
    "ВОЛЬТ/ДЕЛ 1", "Range 1",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showRange[Chan::A], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_RangeB,                                                                                                  //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 2 ---
    "ВОЛЬТ/ДЕЛ 2", "Range 2",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showRange[Chan::B], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_TrigParam,                                                                                             //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. синхр. ---
    "Парам. синхр.", "Trig param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showTrigParam, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_ChanParamA,                                                                                            //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2 ---
    "Парам. кан. 1", "Chan 1 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showChanParam[Chan::A], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_ChanParamB,                                                                                            //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2 ---
    "Парам. кан. 2", "Chan 2 param",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showChanParam[Chan::B], pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_TBase,                                                                                                     //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВРЕМЯ/ДЕЛ ---
    "ВРЕМЯ/ДЕЛ", "TBase",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showTBase, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cConsole_Registers_TShift,                                                                                                        //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Т см. ---
    "Т см.", "tShift",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    set.dbg_showTShift, pppConsole_Registers, IsActive_Console_Registers, FuncChangedChoice, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_12(pppConsole_Registers,                                                                                                                      //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ ---
    "РЕГИСТРЫ", "REGISTERS",
    "",
    "",
    &cConsole_Registers_ShowAll,     // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Показывать все
    &cConsole_Registers_RD_FL,       // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - RD_FL
    &cConsole_Registers_RShiftA,     // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 1к
    &cConsole_Registers_RShiftB,     // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 2к
    &cConsole_Registers_TrigLev,     // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U синхр
    &cConsole_Registers_RangeA,      // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 1
    &cConsole_Registers_RangeB,      // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 2
    &cConsole_Registers_TrigParam,   // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. синхр.
    &cConsole_Registers_ChanParamA,  // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 1
    &cConsole_Registers_ChanParamB,  // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2
    &cConsole_Registers_TBase,       // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВРЕМЯ/ДЕЛ
    &cConsole_Registers_TShift,      // ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Т см.
    Page::Name::Debug_Console_Registers, &pageConsole, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Console_SizeSettings(int x, int y)
{
    char buffer[30];
    sprintf(buffer, "Разм.настр. %d", sizeof(Settings));
    Painter::DrawText(x + 6, y + 13, buffer, Color::BACK);
}

DEF_BUTTON(bConsole_SizeSettings,                                                                                                               //--- ОТЛАДКА - КОНСОЛЬ - Размер настроек ---
    "", "",
    "Показывает текущий размер структуры для сохранения настроек",
    "Displays the current size of the structure to save settings",
    pageConsole, FuncActive, FuncPress, Draw_Console_SizeSettings
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(pageConsole,                                                                                                                                             //--- ОТЛАДКА - КОНСОЛЬ ---
    "КОНСОЛЬ", "CONSOLE",
    "",
    "",
    &gConsole_NumStrings,       ///< ОТЛАДКА - КОНСОЛЬ - Число строк
    &cConsole_SizeFont,         ///< ОТЛАДКА - КОНСОЛЬ - Размер шрифта
    &cConsole_ModeStop,         ///< ОТЛАДКА - КОНСОЛЬ - Реж. останова
    &pppConsole_Registers,      ///< ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ
    &bConsole_SizeSettings,     ///< ОТЛАДКА - КОНСОЛЬ - Размер настроек
    Page::Name::Debug_Console, PageDebug::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

