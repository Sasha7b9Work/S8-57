#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Menu.h"


extern const PageBase pageMeasures;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageMeasures::pointer = &pageMeasures;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pageMeasures, // -V1027                                                                                                                                       //--- ÈÇÌÅĞÅÍÈß ---
    "ÈÇÌÅĞÅÍÈß",
    "Äîñòóï ê íàñòğîéêàì èçìåğåíèé - êóğñîğíûõ è àâòîìàòè÷åñêèõ",
    PageMeasures::PageCursors::pointer,     ///< ÈÇÌÅĞÅÍÈß - ÊÓĞÑÎĞÛ
    PageMeasures::PageAuto::pointer,        ///< ÈÇÌÅĞÅÍÈß - ÀÂÒÎÌÀÒ
    PageMeasures::PageMath::pointer,        ///< ÈÇÌÅĞÅÍÈß - ÌÀÒÅÌÀÒÈÊÀ
    Page::Name::Measures, nullptr, Page::FuncActive, Page::FuncPress, Page::FuncDraw, FuncRegSetPage
)
