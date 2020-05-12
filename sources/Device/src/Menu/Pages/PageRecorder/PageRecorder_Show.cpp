#include "defines.h"
#include "Display/Symbols.h"
#include "Display/Primitives.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Recorder/Recorder.h"
#include "Recorder/DisplayRecorder.h"
#include "Recorder/StorageRecorder.h"
#include "Settings/Settings.h"


static void Draw_Right(int x, int y)
{
    for (int i = 0; i < 2; i++)
    {
        Char(SymbolUGO2::TRIANGLE_RIGHT).Draw4SymbolsInRect(x + i * 12, y + 2);
    }
}

DEF_GRAPH_BUTTON( bScreenRight,                                                                                                     //--- ÔÓÍÊÖÈß - ÐÅÃÈÑÒÐÀÒÎÐ - ÏÐÎÑÌÎÒÐ - Ýêðàí âïðàâî ---
    "Ýêðàí âïðàâî",
    "Ïåðåìåñòèòü îêíî ïðîñìîòðà íà îäèí ýêðàí âïðàâî",
    &PageRecorder::Show::self, Item::Active, DisplayRecorder::MoveWindowRight, Draw_Right
)


static void Draw_Left(int x, int y)
{
    for (int i = 0; i < 2; i++)
    {
        Char(SymbolUGO2::TRIANGLE_LEFT).Draw4SymbolsInRect(x + i * 12, y + 2);
    }
}

DEF_GRAPH_BUTTON( bScreenLeft,                                                                                                       //--- ÔÓÍÊÖÈß - ÐÅÃÈÑÒÐÀÒÎÐ - ÏÐÎÑÌÎÒÐ - Ýêðàí âëåâî ---
    "Ýêðàí âëåâî",
    "Ïåðåìåñòèòü îêíî ïðîñìîòðà íà îäèí ýêðàí âëåâî",
    &PageRecorder::Show::self, Item::Active, DisplayRecorder::MoveWindowLeft, Draw_Left
)


static bool IsActive_PageShow()
{
    return !Recorder::InRecordingMode();
}

static bool HandlerKey_PageShow(const KeyEvent &event)
{
    if (event.IsPress() || event.IsRepeat())
    {
        if (event.IsArrowLeft())
        {
            DisplayRecorder::MoveCursorLeft();
            return true;
        }
        else if (event.IsArrowRight())
        {
            DisplayRecorder::MoveCursorRight();
            return true;
        }
    }

    return false;
}

DEF_PAGE_4( pShow,                                                                                                                                 //--- ÔÓÍÊÖÈß - ÐÅÃÈÑÒÐÀÒÎÐ - ÏÐÎÑÌÎÒÐ ---
    "ÏÐÎÑÌÎÒÐ",
    "Ïðîñìîòð çàïèñàííûõ äàííûõ",
    PageRecorder::Show::Choice::self,
    &bScreenLeft,
    &bScreenRight,
    PageRecorder::Show::Cursors::self,
    PageName::Recorder_Show, &PageRecorder::self, IsActive_PageShow, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, HandlerKey_PageShow
)

const Page * const PageRecorder::Show::self = static_cast<const Page *>(&pShow);
