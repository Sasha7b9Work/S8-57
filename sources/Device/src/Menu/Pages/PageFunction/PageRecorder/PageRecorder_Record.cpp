#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"


extern const PageBase pageRecord;

const Page * const PageFunction::PageRecorder::PageRecord::self = (const Page *)&pageRecord;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_3( cDestination,                                                                                                            //--- ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ - Сохранять в ---
    "Сохранять в",
    "Куда сохранять данные",
    "ОЗУ",
    "USB-Flash",
    "USB-PC",
    RECORDER_STORAGE_RECORD, &PageFunction::PageRecorder::PageRecord::self, 0, 0, 0
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_1( pageRecord, // -V641 // -V1027                                                                                                           //--- ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ ---
    "ЗАПИСЬ",
    "Выбор места хранения данных",
    &cDestination,        ///< ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ - Сохранять в
    Page::Name::Function_Recorder_Record, &PageFunction::PageRecorder::self, 0, 0, 0, 0
)
