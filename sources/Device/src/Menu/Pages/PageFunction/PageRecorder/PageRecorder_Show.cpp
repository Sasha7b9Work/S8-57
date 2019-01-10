#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageShow;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_3( cSource,                                                                                                                  //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Источник ---
    "Источник", "Source",
    "Место расположения данных",
    "Location of data",
    "ОЗУ", "RAM",
    "USB-Flash", "USB-Flash",
    "USB-PC",    "USB-PC",
    RECORDER_STORAGE_PLAY, pageShow, FuncActive, FuncChangedChoice, FuncDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Next()
{

}

DEF_BUTTON( bNext,
    "Следующий", "Next",
    "Предыдущий", "Previous",
    pageShow, FuncActive, OnPress_Next, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageShow, // -V641 // -V1027                                                                                                           //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР ---
    "ПРОСМОТР", "VIEW",
    "Просмотр записанных данных",
    "View recorded data",
    &cSource,           ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Источник
    &bNext,             ///< ФУНКЦИЯ - РЕГИСТРАТОР _ ПРОСМОТР - Следующий
    Page::Name::Function_Recorder_Play, PageFunction::PageRecorder::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageShow::pointer = &pageShow;
