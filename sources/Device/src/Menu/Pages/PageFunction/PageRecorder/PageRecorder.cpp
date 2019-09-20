#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Device.h"


extern const Page pageRecorder;

const Page * const PageRecorder::self = (const Page *)&pageRecorder;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_3( cDestination,                                                                                                                     //--- ФУНКЦИЯ - РЕГИСТРАТОР - Сохранять в ---
    "Сохранять в",
    "Куда сохранять данные",
    "ОЗУ",
    "ПЗУ",
    "Внешн ЗУ",
    RECORDER_STORAGE_RECORD, &PageRecorder::self, E_BtV, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnOpenClose_Recorder(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}

DEF_PAGE_3( pageRecorder, // -V641 // -V1027                                                                                                                  //--- ФУНКЦИЯ - РЕГИСТРАТОР ---
    "РЕГИСТРАТОР",
    "Запись и воспроизведение сигналов входов и датчиков",
    PageRecorder::PageSource::self,     ///< ФУНКЦИЯ - РЕГИСТРАТОР - ИСТОЧНИК
    &cDestination,                      ///< ФУНКЦИЯ - РЕГИСТРАТОР - Сохранять в
    PageRecorder::PageShow::self,       ///< ФУНКЦИЯ - РЕГИСТРАТОР - ПРОСМОТР
    PageName::Function_Recorder, &PageFunction::self, E_BtV, OnOpenClose_Recorder, E_VV, E_BfKE
)
