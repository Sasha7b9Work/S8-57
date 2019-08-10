#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Device.h"


extern const PageBase pageRecorder;

const Page * const PageRecorder::self = (const Page *)&pageRecorder;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DEF_CHOICE_2( cShow,                                                                                                                                 //--- ФУНКЦИЯ - РЕГИСТРАТОР - Всегда ---
//    "Показывать",
//    "Позволяет выводить данные регистратора при выключенном меню РЕГИСТРАТОР",
//    "В режиме",
//    "Всегда",
//    RECORDER_VIEW_ALLWAYS, &PageRecorder::self, 0, 0, 0
//)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnOpenClose_Recorder(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}


DEF_PAGE_2( pageRecorder, // -V641 // -V1027                                                                                                                  //--- ФУНКЦИЯ - РЕГИСТРАТОР ---
    "РЕГИСТРАТОР",
    "Запись и воспроизведение сигналов входов и датчиков",
    //&cShow,                                             ///< ФУНКЦИЯ - РЕГИСТРАТОР - Показывать
    PageRecorder::PageSource::self,    ///< ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК
    //PageRecorder::PageRecord::self,    ///< ФУНКЦИЯ - РЕГИСТРАТОР - ЗАПИСЬ
    PageRecorder::PageShow::self,      ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР
    Page::Name::Function_Recorder, &PageFunction::self, 0, OnOpenClose_Recorder, 0, 0
)
