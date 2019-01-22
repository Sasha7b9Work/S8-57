#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"


extern const PageBase pageShow;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_3( cSource,                                                                                                                  //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Источник ---
    "Источник", "Source",
    "Место расположения данных",
    "Location of data",
    "ОЗУ", "RAM",
    "USB-Flash", "USB-Flash",
    "USB-PC",    "USB-PC",
    RECORDER_STORAGE_PLAY, pageShow, FuncActive, Choice::EmptyChange, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Next()
{

}

DEF_BUTTON( bNext,                                                                                                                     //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Следующий ---
    "Следующий", "Next",
    "",
    "",
    pageShow, FuncActive, OnPress_Next, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Prev()
{

}

DEF_BUTTON( bPrev,                                                                                                                    //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Предыдущий ---
    "Предыдущий", "Previous",
    "",
    "",
    pageShow, FuncActive, OnPress_Prev, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5( pageShow, // -V641 // -V1027                                                                                                           //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР ---
    "ПРОСМОТР", "VIEW",
    "Просмотр записанных данных",
    "View recorded data",
    &cSource,                                                       ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Источник
    &bNext,                                                         ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Следующий
    &bPrev,                                                         ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Предыдущий
    PageFunction::PageRecorder::PageShow::PageOperations::pointer,  ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - ОПЕРАЦИИ
    PageFunction::PageRecorder::PageShow::PageView::pointer,        ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМТОР - ПРОСМОТР
    Page::Name::Function_Recorder_Show, PageFunction::PageRecorder::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageShow::pointer = &pageShow;
