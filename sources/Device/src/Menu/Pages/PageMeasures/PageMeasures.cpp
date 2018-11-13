#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Menu.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageMeasures;

const PageBase *PageMeasures::pointer = &pageMeasures;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageMeasures,                                                                                                          //--- ÈÇÌÅĞÅÍÈß ---
    "ÈÇÌÅĞÅÍÈß", "MEASURES",
    "Äîñòóï ê íàñòğîéêàì èçìåğåíèé - êóğñîğíûõ è àâòîìàòè÷åñêèõ",
    "Access to measurement settings - cursor and automatic",
    PageMeasures::PageCursors::pointer,     ///< ÈÇÌÅĞÅÍÈß - ÊÓĞÑÎĞÛ
    PageMeasures::PageAuto::pointer,        ///< ÈÇÌÅĞÅÍÈß - ÀÂÒÎÌÀÒ
    Page::Name::Measures, Menu::pageMain, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

