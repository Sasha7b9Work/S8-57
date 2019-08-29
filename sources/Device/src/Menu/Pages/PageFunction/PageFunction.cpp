#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/Beeper.h"
#include "Display/Painter.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_5( pFunction,                                                                                                                                                      //--- ÔÓÍÊÖÈß ---
    "ÔÓÍÊÖÈß",
    "Âûáîğ ôóíêöèè",
    PageFrequencyCounter::self,   ///< ÔÓÍÊÖÈß - ×ÀÑÒÎÒÎÌÅĞ
    PageFFT::self,                ///< ÔÓÍÊÖÈß - ÑÏÅÊÒĞ
    PageMultimeter::self,         ///< ÔÓÍÊÖÈß - ÌÓËÜÒÈÌÅÒĞ
    PageTester::self,             ///< ÔÓÍÊÖÈß - ÒÅÑÒÅĞ
    PageRecorder::self,           ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ
    PageName::Function, nullptr, E_BtV, E_VB, E_VV, E_BfKE
)

const Page * const PageFunction::self = (const Page *)&pFunction;
