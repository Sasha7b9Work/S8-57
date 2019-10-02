#include "defines.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(cChoice,
    "Курсор",
    "Выбор курсора",
    "1",
    "2",
    set.rec.currentCursor, &PageRecorder::PageShow::PageCursors::self, Choice::Active, Choice::Changed, Choice::AfterDraw
)

DEF_PAGE_1(pCursors,
    "КУРСОРЫ",
    "Работа курсорами",
    &cChoice,
    PageName::Recorder_Show_Choice_Cursors,
    &PageRecorder::PageShow::self, Item::Active, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageRecorder::PageShow::PageCursors::self = (const Page *)&pCursors;
