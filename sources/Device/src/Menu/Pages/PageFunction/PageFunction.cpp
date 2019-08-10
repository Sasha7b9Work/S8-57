#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageFunction;

const Page * const PageFunction::self = (const Page *)&pageFunction;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5( pageFunction,                                                                                                                                                   //--- ÔÓÍÊÖÈß ---
    "ÔÓÍÊÖÈß",
    "Âûáîğ ôóíêöèè",
    PageFunction::PageFrequencyCounter::self,   ///< ÔÓÍÊÖÈß - ×ÀÑÒÎÒÎÌÅĞ
    PageFunction::PageFFT::self,                ///< ÔÓÍÊÖÈß - ÑÏÅÊÒĞ
    PageFunction::PageMultimeter::self,         ///< ÔÓÍÊÖÈß - ÌÓËÜÒÈÌÅÒĞ
    PageFunction::PageTester::self,             ///< ÔÓÍÊÖÈß - ÒÅÑÒÅĞ
    PageFunction::PageRecorder::self,           ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ
    Page::Name::Function, nullptr, 0, 0, 0, 0
)
