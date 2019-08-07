#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageRecord;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_3( cDestination,                                                                                                            //--- ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ - Сохранять в ---
    "Сохранять в",
    "Куда сохранять данные",
    "ОЗУ",
    "USB-Flash",
    "USB-PC",
    RECORDER_STORAGE_RECORD, pageRecord, Choice::FuncActive, Choice::FuncChange, Button::EmptyDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_1( pageRecord, // -V641 // -V1027                                                                                                           //--- ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ ---
    "ЗАПИСЬ",
    "Выбор места хранения данных",
    &cDestination,        ///< ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ - Сохранять в
    Page::Name::Function_Recorder_Record, PageFunction::PageRecorder::pointer, Page::FuncActive, Page::FuncPress, Page::FuncDraw, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageRecord::pointer = &pageRecord;
