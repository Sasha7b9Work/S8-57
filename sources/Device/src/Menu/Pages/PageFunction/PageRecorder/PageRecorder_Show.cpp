#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder_Storage.h"
#include "Recorder/Recorder_Display.h"


extern const PageBase pageShow;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_3( cSource,                                                                                                                  //--- ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - Èñòî÷íèê ---
    "Èñòî÷íèê",
    "Ìåñòî ğàñïîëîæåíèÿ äàííûõ",
    "ÎÇÓ",
    "USB-Flash",
    "USB-PC",
    RECORDER_STORAGE_PLAY, pageShow, Choice::FuncActive, Choice::FuncChange, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cCursor,
    "Êóğñîğ",
    "",
    "1",
    "2",
    Recorder::Settings::currentCursor, pageShow, Choice::FuncActive, Choice::FuncChange, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Next()
{
    Recorder::Display::MoveLeft();
}

DEF_BUTTON( bNext,                                                                                                                     //--- ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - Ñëåäóşùèé ---
    "Âëåâî",
    "",
    pageShow, FuncActive, OnPress_Next, Button::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Prev()
{
    Recorder::Display::MoveRight();
}

DEF_BUTTON( bPrev,                                                                                                                    //--- ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - Ïğåäûäóùèé ---
    "Âïğàâî",
    "",
    pageShow, FuncActive, OnPress_Prev, Button::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_PageShow()
{
    return (Recorder::Storage::CurrentFrame()->NumPoints() != 0) && (!Recorder::IsRunning());
}

static bool FuncOnKey_PageShow(KeyEvent event)
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

DEF_PAGE_3( pageShow, // -V641 // -V1027                                                                                                           //--- ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ ---
    "ÏĞÎÑÌÎÒĞ",
    "Ïğîñìîòğ çàïèñàííûõ äàííûõ",
    //&cSource,                                                       ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - Èñòî÷íèê
    &bNext,                                                         ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - Ñëåäóşùèé
    &bPrev,                                                         ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - Ïğåäûäóùèé
    &cCursor,
    //PageFunction::PageRecorder::PageShow::PageOperations::pointer,  ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ - ÎÏÅĞÀÖÈÈ
    //PageFunction::PageRecorder::PageShow::PageView::pointer,        ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÒÎĞ - ÏĞÎÑÌÎÒĞ
    Page::Name::Function_Recorder_Show, PageFunction::PageRecorder::pointer, IsActive_PageShow, FuncPressPage, FuncDrawPage, FuncOnKey_PageShow
)

const PageBase *PageFunction::PageRecorder::PageShow::pointer = &pageShow;
