#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Menu/MenuItems.h"
#include "Utils/CommonFunctions.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageHelp.h"
#include "Menu/Pages/Include/HelpContent.h"


using namespace Display::Primitives;

extern const Page pHelp;

const Page * const PageHelp::self = (const Page *)&pHelp;



static void Draw_Enter(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x4a').Draw4SymbolsInRect(x + 2, y + 2);
    Font::SetCurrent(Font::Type::_8);
}


static void Draw_Leave(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x48').Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}


static void Draw_Prev(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x4c').Draw4SymbolsInRect(x + 2, y + 5);
    Font::SetCurrent(Font::Type::_8);
}


static void Draw_Next(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char('\x4e').Draw4SymbolsInRect(x + 2, y + 5);
    Font::SetCurrent(Font::Type::_8);
}


static bool OnArrows_Help(const KeyEvent &)
{
    return true;
}


DEF_GRAPH_BUTTON( bEnter,                                                                                                                                          //--- ПОМОЩЬ - Открыть ---
    "Открыть",
    "Открывает раздел справки",
    &PageHelp::self, HelpContent::EnterParagraphIsActive, HelpContent::EnterParagraph, Draw_Enter
)


DEF_GRAPH_BUTTON( bLeave,                                                                                                                                          //--- ПОМОЩЬ - Закрыть ---
     "Закрыть",
     "Закрывает раздел справки",
    &PageHelp::self, HelpContent::LeaveParagraphIsActive, HelpContent::LeaveParagraph, Draw_Leave
)


DEF_GRAPH_BUTTON( bPrev,                                                                                                                                 //--- ПОМОЩЬ - Предыдущий раздел ---
    "Предыдущий раздел",
    "Выбрать предыдущий раздел справки",
    &PageHelp::self, Item::Active, HelpContent::PrevParagraph, Draw_Prev
)


DEF_GRAPH_BUTTON( bNext,                                                                                                                    //--- ПОМОЩЬ - Следующий раздел ---
    "Следующий раздел",
    "Выбрать следующий раздел справки",
    &PageHelp::self, Item::Active, HelpContent::NextParagraph, Draw_Next
)

/*
static void PressSB_Help_Exit()
{

}
*/


/*
DEF_GRAPH_BUTTON_EXIT(  sbExitHelp,                                                                                                                                  //--- ПОМОЩЬ - Выход ---
    pHelp, 0, PressSB_Help_Exit, DrawSB_Help_ParagraphNext
)
*/

/*
DEF_PAGE_SB(        pHelp,                                                                                                                                                   //--- ПОМОЩЬ ---
    "ПОМОЩЬ", "HELP",
    "Открыть разделы помощи",
    "To open sections of the help",
    &sbExitHelp,
    &bEnter,
    &bLeave,
    0,
    &bPrev,
    &bNext,
    Page::Name::SB_Help, Menu::pageMain, 0, 0, HelpContent_Draw, HandlerKey_Help
)
*/

DEF_PAGE_4( pHelp, // -V641 // -V1027                                                                                                                                        //--- ПОМОЩЬ ---
    "ПОМОЩЬ",
    "Открыть разделы помощи",
    &bEnter,
    &bLeave,
    &bPrev,
    &bNext,
    PageName::Help, nullptr, Item::Active, Page::Changed, HelpContent::Draw, OnArrows_Help
)
