#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pagePlay;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cSource,                                                                            //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Источник ---
    "Источник", "Source",
    "Место расположения данных",
    "Location of data",
    "USB-PC",    "USB-PC",
    "USB-Flash", "USB-Flash",
    RECORDER_STORAGE_PLAY, pagePlay, FuncActive, FuncChangedChoice, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_1(pagePlay,                                                                                        //--- ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР ---
    "ПРОСМОТР", "VIEW",
    "Просмотр записанных данных",
    "View recorded data",
    &cSource,                   ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР - Источник
    Page::Name::Function_Recorder_Play, PageFunction::PageRecorder::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PagePlay::pointer = &pagePlay;
