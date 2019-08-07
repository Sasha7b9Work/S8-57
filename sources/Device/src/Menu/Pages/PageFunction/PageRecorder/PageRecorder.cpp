#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Device.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageRecorder;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cShow,                                                                                                                                 //--- ФУНКЦИЯ - РЕГИСТРАТОР - Всегда ---
    "Показывать",
    "Позволяет выводить данные регистратора при выключенном меню РЕГИСТРАТОР",
    "В режиме",
    "Всегда",
    RECORDER_VIEW_ALLWAYS, pageRecorder, 0, 0, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnOpenClose_Recorder(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}


DEF_PAGE_2( pageRecorder, // -V641 // -V1027                                                                                                                  //--- ФУНКЦИЯ - РЕГИСТРАТОР ---
    "РЕГИСТРАТОР",
    "Запись и воспроизведение сигналов входов и датчиков",
    //&cShow,                                             ///< ФУНКЦИЯ - РЕГИСТРАТОР - Показывать
    PageFunction::PageRecorder::PageSource::pointer,    ///< ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК
    //PageFunction::PageRecorder::PageRecord::pointer,    ///< ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ
    PageFunction::PageRecorder::PageShow::pointer,      ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР
    Page::Name::Function_Recorder, PageFunction::pointer, 0, OnOpenClose_Recorder, 0, 0
)

const PageBase *PageFunction::PageRecorder::pointer = &pageRecorder;
