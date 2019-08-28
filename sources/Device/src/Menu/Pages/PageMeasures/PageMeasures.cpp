#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Menu.h"


extern const Page pageMeasures;

const Page * const PageMeasures::self = (const Page *)&pageMeasures;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3( pageMeasures, // -V1027                                                                                                                                       //--- ÈÇÌÅĞÅÍÈß ---
    "ÈÇÌÅĞÅÍÈß",
    "Äîñòóï ê íàñòğîéêàì èçìåğåíèé - êóğñîğíûõ è àâòîìàòè÷åñêèõ",
    PageMeasuresCursors::self,     ///< ÈÇÌÅĞÅÍÈß - ÊÓĞÑÎĞÛ
    PageMeasuresAuto::self,        ///< ÈÇÌÅĞÅÍÈß - ÀÂÒÎÌÀÒ
    PageMeasuresMath::self,        ///< ÈÇÌÅĞÅÍÈß - ÌÀÒÅÌÀÒÈÊÀ
    PageName::Measures, nullptr, 0, 0, 0, 0
)
