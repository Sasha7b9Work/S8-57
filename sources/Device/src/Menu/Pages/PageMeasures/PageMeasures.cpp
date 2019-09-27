#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Menu.h"


extern const Page pMeasures;

const Page * const PageMeasures::self = (const Page *)&pMeasures;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3( pMeasures, // -V1027                                                                                                                                          //--- ÈÇÌÅĞÅÍÈß ---
    "ÈÇÌÅĞÅÍÈß",
    "Äîñòóï ê íàñòğîéêàì èçìåğåíèé - êóğñîğíûõ è àâòîìàòè÷åñêèõ",
    PageMeasuresCursors::self,     ///< ÈÇÌÅĞÅÍÈß - ÊÓĞÑÎĞÛ
    PageMeasuresAuto::self,        ///< ÈÇÌÅĞÅÍÈß - ÀÂÒÎÌÀÒ
    PageMeasuresMath::self,        ///< ÈÇÌÅĞÅÍÈß - ÌÀÒÅÌÀÒÈÊÀ
    PageName::Measures, nullptr, Item::EmptyActive, E_VB, Page::EmptyBeforeDraw, E_BfKE
)
