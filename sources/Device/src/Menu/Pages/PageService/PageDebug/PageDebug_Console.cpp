#include "defines.h"
#include "Display/Colors.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageService.h"
#include "Settings/Settings.h"



DEF_CHOICE_2( cShow,                                                                                                                                 //--- ОТЛАДКА - КОНСОЛЬ - Показывать ---
    "Показывать",
    "",
    "Нет",
    "Да",
    set.dbg.showConsole, &PageDebug::PageConsole::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_GOVERNOR( gNumStrings,                                                                                                                          //--- ОТЛАДКА - КОНСОЛЬ - Число строк ---
    "Число строк",
    "",
    set.dbg.numStrings, 0, 33, &PageDebug::PageConsole::self, Item::Active, Console::OnChanged_MaxStringsInConsole
)


DEF_CHOICE_2( cShowAll,                                                                                                               //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Показывать все ---
    "Показывать все",
    "Показывать все значения, засылаемые в регистры",
    "Нет",
    "Да",
    set.dbg.showAll, &PageDebug::PageRegisters::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Registers()
{
    return set.dbg.showAll;
}

DEF_CHOICE_2( cRD_FL,                                                                                                                          //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - RD_FL ---
    "RD_FL",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showFlag, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRShiftA,                                                                                                                     //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 1к ---
    "U см. 1к",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRShift[Chan::A], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRShiftB,                                                                                                                     //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U см. 2к ---
    "U см. 2к",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRShift[Chan::B], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cTrigLev,                                                                                                                     //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - U синхр. ---
    "U синхр.",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showTrigLev, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRangeA,                                                                                                                   //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 1 ---
    "ВОЛЬТ/ДЕЛ 1",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRange[Chan::A], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cRangeB,                                                                                                                   //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВОЛЬТ/ДЕЛ 2 ---
    "ВОЛЬТ/ДЕЛ 2",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRange[Chan::B], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cTrigParam,                                                                                                              //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. синхр. ---
    "Парам. синхр.",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showTrigParam, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cChanParamA,                                                                                                             //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2 ---
    "Парам. кан. 1",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showChanParam[Chan::A], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cChanParamB,                                                                                                             //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Парам. кан. 2 ---
    "Парам. кан. 2",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showChanParam[Chan::B], &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cTBase,                                                                                                                      //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - ВРЕМЯ/ДЕЛ ---
    "ВРЕМЯ/ДЕЛ",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showTBase, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cTShift,                                                                                                                         //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ - Т см. ---
    "Т см.",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showTShift, &PageDebug::PageRegisters::self, IsActive_Registers, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_12( pRegisters,                                                                                                                               //--- ОТЛАДКА - КОНСОЛЬ - РЕГИСТРЫ ---
    "РЕГИСТРЫ",
    "",
    &cShowAll,
    &cRD_FL,
    &cRShiftA,
    &cRShiftB,
    &cTrigLev,
    &cRangeA,
    &cRangeB,
    &cTrigParam,
    &cChanParamA,
    &cChanParamB,
    &cTBase,
    &cTShift,
    PageName::Debug_Console_Registers, &PageDebug::PageConsole::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageRegisters::self = static_cast<const Page *>(&pRegisters);


DEF_CHOICE_2( cShowGate,
    "Ворота",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRandGate, &PageDebug::PageConsole::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

DEF_CHOICE_2( cShowPredPost,
    "Предзапуск",
    "",
    DISABLE_RU,
    ENABLE_RU,
    set.dbg.showRandPredPost, &PageDebug::PageConsole::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_4( pConsole,                                                                                                                                             //--- ОТЛАДКА - КОНСОЛЬ ---
    "КОНСОЛЬ",
    "Управляет отображением и параметрами отладочной консоли",
    &cShow,
    &gNumStrings,
    &cShowGate,
    &cShowPredPost,
    PageName::Debug_Console, &PageDebug::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageConsole::self = static_cast<const Page *>(&pConsole);
