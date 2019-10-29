#include "defines.h"
#include "Display/Symbols.h"
#include "Display/Primitives.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(cChoice,
    "Курсор",
    "Выбор курсора",
    "1",
    "2",
    set.rec.currentCursor, &PageRecorder::PageShow::PageCursors::self, Choice::Active, Choice::Changed, Choice::AfterDraw
)


static void OnPress_Right()
{

}

static void Draw_Right(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_RIGHT).Draw4SymbolsInRect(x + 6, y + 2);
}

DEF_GRAPH_BUTTON( bRight,
    "Вправо",
    "Перемещение курсора вправо",
    &PageRecorder::PageShow::PageCursors::self, Item::Active, OnPress_Right, Draw_Right
)


static void OnPress_Left()
{

}

static void Draw_Left(int x, int y)
{
    Char(SymbolUGO2::TRIANGLE_LEFT).Draw4SymbolsInRect(x + 6, y + 2);
}

DEF_GRAPH_BUTTON( bLeft,
    "Влево",
    "Перемещение курсора влево",
    &PageRecorder::PageShow::PageCursors::self, Item::Active, OnPress_Left, Draw_Left
)


DEF_PAGE_3(pCursors,
    "КУРСОРЫ",
    "Работа курсорами",
    &cChoice,
    &bLeft,
    &bRight,
    PageName::Recorder_Show_Choice_Cursors,
    &PageRecorder::PageShow::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageRecorder::PageShow::PageCursors::self = static_cast<const Page *>(&pCursors);
