#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFFT.h"
#include "Menu/Pages/Include/PageFreqMeter.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMultimeter.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Menu/Pages/Include/PageTester.h"


DEF_PAGE_5( pFunction,                                                                                                                                                      //--- ÔÓÍÊÖÈß --- //-V2567
    "ÔÓÍÊÖÈß",
    "Âûáîğ ôóíêöèè",
    PageFreqMeter::self,
    PageFFT::self,
    PageMultimeter::self,
    PageTester::self,
    PageRecorder::self,
    PageName::Function, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageFunction::self = static_cast<const Page *>(&pFunction);
