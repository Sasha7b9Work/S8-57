#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Menu.h"


extern const PageBase pageMeasures;

const Page * const PageMeasures::self = (const Page * const)&pageMeasures;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3( pageMeasures, // -V1027                                                                                                                                       //--- ÈÇÌÅĞÅÍÈß ---
    "ÈÇÌÅĞÅÍÈß",
    "Äîñòóï ê íàñòğîéêàì èçìåğåíèé - êóğñîğíûõ è àâòîìàòè÷åñêèõ",
    PageMeasures::PageCursors::self,     ///< ÈÇÌÅĞÅÍÈß - ÊÓĞÑÎĞÛ
    PageMeasures::PageAuto::self,        ///< ÈÇÌÅĞÅÍÈß - ÀÂÒÎÌÀÒ
    PageMeasures::PageMath::self,        ///< ÈÇÌÅĞÅÍÈß - ÌÀÒÅÌÀÒÈÊÀ
    Page::Name::Measures, nullptr, 0, 0, 0, 0
)
