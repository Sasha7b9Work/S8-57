#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageRecord;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cDestinationSource,                                                                                                      //--- ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ - Сохранять в ---
    "Сохранять в", "Save to",
    "Куда сохранять данные",
    "Where to save data",
    "USB-PC",    "USB-PC",
    "USB-Flash", "USB-Flash",
    RECORDER_STORAGE_RECORD, pageRecord, FuncActive, FuncChangedChoice, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_1( pageRecord, // -V641 // -V1027                                                                                                           //--- ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ ---
    "ЗАПИСЬ", "RECORD",
    "Доступ к записи данных",
    "Access to data record",
    &cDestinationSource,        ///< ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ - Сохранять в
    Page::Name::Function_Recorder_Record, PageFunction::PageRecorder::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageRecord::pointer = &pageRecord;
