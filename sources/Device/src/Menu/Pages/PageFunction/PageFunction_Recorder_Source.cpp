#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageSource;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cSourceA,                                                                            //--- ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК - Канал 1 ---
    "Канал 1", "Channl 1",
    "Выбор канала 1 для записи и просмотра",
    "Select channel 1 for recording and viewing",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    RECORDER_SOURCE_A, pageSource, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cSourceB,                                                                            //--- ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧИНК - Канал 2 ---
    "Канал 2", "Channl 2",
    "Выбор канала 2 для записи и просмотра",
    "Select channel 2 for recording and viewing",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    RECORDER_SOURCE_B, pageSource, FuncActive, FuncChangedChoice, FuncDraw
)

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cSourceSensor,                                                                        //--- ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧИНК - Датчик ---
    "Датчик", "Sensor",
    "Выбор датчика для записи и просмотра",
    "Sensor selection for recording and viewing",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    RECORDER_SOURCE_SENSOR, pageSource, FuncActive, FuncChangedChoice, FuncDraw
)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pageSource,                                                                                     //--- ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК ---
    "ИСТОЧНИК", "SOURCE",
    "Выбор записываемых сигналов",
    "Selection of recorded signals",
    &cSourceA,       ///< ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК - Канал 1
    &cSourceB,       ///< ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧИНК - Канал 2
    &cSourceSensor,  ///< ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК - Датчик
    Page::Name::Function_Recorder_Source, PageFunction::PageRecorder::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageSource::pointer = &pageSource;
