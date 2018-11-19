#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Display/Painter.h"
#include "Menu/MenuItems.h"
#include "Utils/CommonFunctions.h"
#include "Menu/Menu.h"
#endif
#include "Menu/Pages/Include/PageHelp.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Pages/Include/HelpContent.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pHelp;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawSB_Help_ParagraphEnter(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x4a');
    Painter::SetFont(Font::Type::_8);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSB_Help_ParagraphLeave(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x48');
    Painter::SetFont(Font::Type::_8);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSB_Help_ParagraphPrev(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 5, '\x4c');
    Painter::SetFont(Font::Type::_8);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSB_Help_ParagraphNext(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 5, '\x4e');
    Painter::SetFont(Font::Type::_8);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool OnHelpRegSet(int)
{
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbHelpParagraphEnter,                                                                                   //--- ПОМОЩЬ - Открыть ---
    "Открыть",  "Open",
    "Открывает раздел справки",
    "Opens the section of the reference",
    pHelp, HelpContent_EnterParagraphIsActive, HelpContent_EnterParagraph, DrawSB_Help_ParagraphEnter
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbHelpParagraphLeave,                                                                                   //--- ПОМОЩЬ - Закрыть ---
     "Закрыть", "Close",
     "Закрывает раздел справки",
     "Closes the section of the reference",
    pHelp, HelpContent_LeaveParagraphIsActive, HelpContent_LeaveParagraph, DrawSB_Help_ParagraphLeave
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbHelpParagraphPrev,                                                                          //--- ПОМОЩЬ - Предыдущий раздел ---
    "Предыдущий раздел", "Previous section",
    "Выбрать предыдущий раздел справки",
    "To choose the previous section of the reference",
    pHelp, FuncActive, HelpContent_PrevParagraph, DrawSB_Help_ParagraphPrev
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_SMALL_BUTTON(   sbHelpParagraphNext,                                                                           //--- ПОМОЩЬ - Следующий раздел ---
    "Следующий раздел", "Next section",
    "Выбрать следующий раздел справки",
    "To choose the next section of the reference",
    pHelp, FuncActive, HelpContent_NextParagraph, DrawSB_Help_ParagraphNext
)

/*
static void PressSB_Help_Exit()
{

}
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
DEF_SMALL_BUTTON_EXIT(  sbExitHelp,                                                                                           //--- ПОМОЩЬ - Выход ---
    pHelp, FuncActive, PressSB_Help_Exit, DrawSB_Help_ParagraphNext
)
*/

const PageBase *PageHelp::pointer = &pHelp;

/*
DEF_PAGE_SB(        pHelp,                                                                                                               // ПОМОЩЬ ///
    "ПОМОЩЬ", "HELP",
    "Открыть разделы помощи",
    "To open sections of the help",
    &sbExitHelp,
    &sbHelpParagraphEnter,
    &sbHelpParagraphLeave,
    0,
    &sbHelpParagraphPrev,
    &sbHelpParagraphNext,
    Page::Name::SB_Help, Menu::pageMain, FuncActive, EmptyPressPage, HelpContent_Draw, OnHelpRegSet
)
*/

DEF_PAGE_4(pHelp,                                                                                                               // ПОМОЩЬ ///
    "ПОМОЩЬ", "HELP",
    "Открыть разделы помощи",
    "To open sections of the help",
    &sbHelpParagraphEnter,
    &sbHelpParagraphLeave,
    &sbHelpParagraphPrev,
    &sbHelpParagraphNext,
    Page::Name::SB_Help, Menu::pageMain, FuncActive, EmptyPressPage, HelpContent_Draw, OnHelpRegSet
)
