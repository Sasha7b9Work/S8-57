#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Device.h"


extern const Page pageRecorder;

const Page * const PageRecorder::self = (const Page *)&pageRecorder;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnOpenClose_Recorder(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}


DEF_PAGE_1( pageRecorder, // -V641 // -V1027                                                                                                                  //--- ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ ---
    "ĞÅÃÈÑÒĞÀÒÎĞ",
    "Çàïèñü è âîñïğîèçâåäåíèå ñèãíàëîâ âõîäîâ è äàò÷èêîâ",
    PageRecorder::PageSource::self,    ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÈÑÒÎ×ÍÈÊ
//    PageRecorder::PageRecord::self,    ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÇÀÏÈÑÜ
//    PageRecorder::PageShow::self,      ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ - ÏĞÎÑÌÎÒĞ
    PageName::Function_Recorder, &PageFunction::self, E_BtV, OnOpenClose_Recorder, E_VV, E_BfKE
)
