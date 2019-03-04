#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder_Storage.h"
#include "Recorder/Recorder_Display.h"


extern const PageBase pageShow;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_3( cSource,                                                                                                                  //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Источник ---
    "Источник", "Source",
    "Место расположения данных",
    "Location of data",
    "ОЗУ", "RAM",
    "USB-Flash", "USB-Flash",
    "USB-PC",    "USB-PC",
    RECORDER_STORAGE_PLAY, pageShow, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Next()
{
    Recorder::Display::MoveLeft();
}

DEF_BUTTON( bNext,                                                                                                                     //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Следующий ---
    "Влево", "Left",
    "",
    "",
    pageShow, FuncActive, OnPress_Next, Button::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Prev()
{
    Recorder::Display::MoveRight();
}

DEF_BUTTON( bPrev,                                                                                                                    //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Предыдущий ---
    "Вправо", "Right",
    "",
    "",
    pageShow, FuncActive, OnPress_Prev, Button::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_PageShow()
{
    return (Recorder::Storage::CurrentFrame()->NumPoints() != 0) && (!Recorder::IsRunning());
}

DEF_PAGE_2( pageShow, // -V641 // -V1027                                                                                                           //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР ---
    "ПРОСМОТР", "VIEW",
    "Просмотр записанных данных",
    "View recorded data",
    //&cSource,                                                       ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Источник
    &bNext,                                                         ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Следующий
    &bPrev,                                                         ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Предыдущий
    //PageFunction::PageRecorder::PageShow::PageOperations::pointer,  ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ОПЕРАЦИИ
    //PageFunction::PageRecorder::PageShow::PageView::pointer,        ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМТОР - ПРОСМОТР
    Page::Name::Function_Recorder_Show, PageFunction::PageRecorder::pointer, IsActive_PageShow, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageShow::pointer = &pageShow;
