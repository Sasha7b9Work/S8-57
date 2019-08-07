#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageFunction;

const PageBase *PageFunction::pointer = &pageFunction;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5( pageFunction,                                                                                                                                                   //--- ÔÓÍÊÖÈß ---
    "ÔÓÍÊÖÈß",
    "Âûáîğ ôóíêöèè",
    PageFunction::PageFrequencyCounter::pointer,   ///< ÔÓÍÊÖÈß - ×ÀÑÒÎÒÎÌÅĞ
    PageFunction::PageFFT::pointer,                ///< ÔÓÍÊÖÈß - ÑÏÅÊÒĞ
    PageFunction::PageMultimeter::pointer,         ///< ÔÓÍÊÖÈß - ÌÓËÜÒÈÌÅÒĞ
    PageFunction::PageTester::pointer,             ///< ÔÓÍÊÖÈß - ÒÅÑÒÅĞ
    PageFunction::PageRecorder::pointer,           ///< ÔÓÍÊÖÈß - ĞÅÃÈÑÒĞÀÒÎĞ
    Page::Name::Function, nullptr, 0, Page::FuncPress, Page::FuncDraw, Page::FuncRegSet
)
