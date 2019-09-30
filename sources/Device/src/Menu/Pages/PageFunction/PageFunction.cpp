#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/Beeper.h"
#include "Display/Painter.h"



DEF_PAGE_5( pFunction,                                                                                                                                                      //--- ÔÓÍÊÖÈß ---
    "ÔÓÍÊÖÈß",
    "Âûáîğ ôóíêöèè",
    PageFreqMeter::self,        ///< ÔÓÍÊÖÈß - ×ÀÑÒÎÒÎÌÅĞ
    PageFFT::self,              ///< ÔÓÍÊÖÈß - ÑÏÅÊÒĞ
    PageMultimeter::self,       ///< ÔÓÍÊÖÈß - ÌÓËÜÒÈÌÅÒĞ
    PageTester::self,           ///< ÔÓÍÊÖÈß - ÒÅÑÒÅĞ
    PageRecorder::self,         ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ
    PageName::Function, nullptr, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageFunction::self = (const Page *)&pFunction;
