#include "defines.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder_Storage.h"
#include "Recorder/Recorder_Display.h"



DEF_CHOICE_3( cSource,
    "Èñòî÷íèê",
    "Îòêóäà áğàòü äàííûå äëÿ ïğîñìîòğà",
    "ÎÇÓ",
    "Âíóòğ ÇÓ",
    "Âíåøí ÇÓ",
    set.rec.storagePlay, &PageRecorder::PageShow::self, Item::Active, Choice::Changed, Choice::AfterDraw
)



DEF_CHOICE_2( cCursor,
    "Êóğñîğ",
    "",
    "1",
    "2",
    set.rec.currentCursor, &PageRecorder::PageShow::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static void OnPress_Next()
{
    Recorder::Display::MoveLeft();
}

DEF_BUTTON( bNext,                                                                                                                     //--- ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - Ñëåäóşùèé ---
    "Âëåâî",
    "",
    &PageRecorder::PageShow::self, Item::Active, OnPress_Next
)


static void OnPress_Prev()
{
    Recorder::Display::MoveRight();
}

DEF_BUTTON( bPrev,                                                                                                                    //--- ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - Ïğåäûäóùèé ---
    "Âïğàâî",
    "",
    &PageRecorder::PageShow::self, Item::Active, OnPress_Prev
)


static bool IsActive_PageShow()
{
    return !Recorder::IsRunning();
}

static bool HandlerKey_PageShow(const KeyEvent &event)
{
    if (event.type == TypePress::Press || event.type == TypePress::Repeat)
    {
        if (event.key == Key::Left)
        {
            Recorder::Display::MoveCursorLeft();
            return true;
        }
        else if (event.key == Key::Right)
        {
            Recorder::Display::MoveCursorRight();
            return true;
        }
        else
        {
            // çäåñü íè÷åãî íå íàäî
        }
    }

    return false;
}

DEF_PAGE_1( pShow, // -V641 // -V1027                                                                                                              //--- ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ ---
    "ÏĞÎÑÌÎÒĞ",
    "Ïğîñìîòğ çàïèñàííûõ äàííûõ",
    &cSource,                                       ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - Èñòî÷íèê
    //&bNext,                                         ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - Ñëåäóşùèé
    //&bPrev,                                         ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - Ïğåäûäóùèé
    //&cCursor,                                       ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - Êóğñîğ
    //PageRecorder::PageShow::PageOperations::self,  ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - ÎÏÅĞÀÖÈÈ
    //PageRecorder::PageShow::PageView::self,        ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÒÎĞ - ÏĞÎÑÌÎÒĞ
    PageName::Function_Recorder_Show, &PageRecorder::self, IsActive_PageShow, Page::Changed, Page::BeforeDraw, HandlerKey_PageShow
)

const Page * const PageRecorder::PageShow::self = (const Page *)&pShow;
