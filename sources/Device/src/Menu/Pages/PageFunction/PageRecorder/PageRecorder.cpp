#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Device.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageRecorder;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cShow,                                                                                                                                 //--- ФУНКЦИЯ - РЕГИСТРАТОР - Всегда ---
    "Показывать", "Show",
    "Позволяет выводить данные регистратора при выключенном меню РЕГИСТРАТОР",
    "Allows you to display data from the recorder when the recorder menu is off",
    "В режиме", "In mode",
    "Всегда",   "In always",
    RECORDER_VIEW_ALLWAYS, pageRecorder, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnOpenClose_Recorder(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}


DEF_PAGE_4( pageRecorder, // -V641 // -V1027                                                                                                                  //--- ФУНКЦИЯ - РЕГИСТРАТОР ---
    "РЕГИСТРАТОР", "RECORDER",
    "Запись и воспроизведение сигналов входов и датчиков",
    "Recording and playback of input signals and sensors",
    &cShow,                                             ///< ФУНКЦИЯ - РЕГИСТРАТОР - Показывать
    PageFunction::PageRecorder::PageSource::pointer,    ///< ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК
    PageFunction::PageRecorder::PageRecord::pointer,    ///< ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ
    PageFunction::PageRecorder::PageShow::pointer,      ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР
    Page::Name::Function_Recorder, PageFunction::pointer, FuncActive, OnOpenClose_Recorder, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::pointer = &pageRecorder;
