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
DEF_CHOICE_3( cDestination,                                                                                                            //--- ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ - Сохранять в ---
    "Сохранять в", "Save to",
    "Куда сохранять данные",
    "Where to save data",
    "ОЗУ",       "RAM",
    "USB-Flash", "USB-Flash",
    "USB-PC",    "USB-PC",
    RECORDER_STORAGE_RECORD, pageRecord, FuncActive, FuncChangedChoice, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_1( pageRecord, // -V641 // -V1027                                                                                                           //--- ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ ---
    "ЗАПИСЬ", "RECORD",
    "Выбор места хранения данных",
    "Choosing a storage location",
    &cDestination,        ///< ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ - Сохранять в
    Page::Name::Function_Recorder_Record, PageFunction::PageRecorder::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageRecord::pointer = &pageRecord;
