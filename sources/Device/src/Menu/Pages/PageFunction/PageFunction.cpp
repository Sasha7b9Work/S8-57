#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageDef pageFunction;

const Page * const PageFunction::self = (const Page *)&pageFunction;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5( pageFunction,                                                                                                                                                   //--- ÔÓÍÊÖÈß ---
    "ÔÓÍÊÖÈß",
    "Âûáîğ ôóíêöèè",
    PageFrequencyCounter::self,   ///< ÔÓÍÊÖÈß - ×ÀÑÒÎÒÎÌÅĞ
    PageFFT::self,                ///< ÔÓÍÊÖÈß - ÑÏÅÊÒĞ
    PageMultimeter::self,         ///< ÔÓÍÊÖÈß - ÌÓËÜÒÈÌÅÒĞ
    PageTester::self,             ///< ÔÓÍÊÖÈß - ÒÅÑÒÅĞ
    PageRecorder::self,           ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ
    Page::Name::Function, nullptr, 0, 0, 0, 0
)
