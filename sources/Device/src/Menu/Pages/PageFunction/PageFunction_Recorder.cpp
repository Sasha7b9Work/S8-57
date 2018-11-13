#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Device.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageRecorder;


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(cViewAlways,                                                                                     //--- ФУНКЦИЯ - РЕГИСТРАТОР - Всегда ---
    "Всегда", "Always",
    "Позволяет выводить данные регистратора при выключенном меню РЕГИСТРАТОР",
    "Allows you to display data from the recorder when the recorder menu is off",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    RECORDER_VIEW_ALLWAYS, pageRecorder, FuncActive, FuncChangedChoice, FuncDraw
)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnEnterExit_Recorder(bool enter)
{
    Device::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}


DEF_PAGE_4( pageRecorder,                                                                                              //--- ФУНКЦИЯ - РЕГИСТРАТОР ---
    "РЕГИСТРАТОР", "RECORDER",
    "Запись и воспроизведение сигналов входов и датчиков",
    "Recording and playback of input signals and sensors",
    &cViewAlways,                                       ///< ФУНКЦИЯ - РЕГИСТРАТОР - Всегда
    PageFunction::PageRecorder::PageSource::pointer,    ///< ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК
    PageFunction::PageRecorder::PageRecord::pointer,    ///< ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ
    PageFunction::PageRecorder::PagePlay::pointer,      ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР
    Page::Name::Function_Recorder, PageFunction::pointer, FuncActive, OnEnterExit_Recorder, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::pointer = &pageRecorder;
