#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Menu.h"


extern const Page pMeasures;

const Page * const PageMeasures::self = (const Page *)&pMeasures;



DEF_PAGE_3( pMeasures, // -V1027                                                                                                                                          //--- ИЗМЕРЕНИЯ ---
    "ИЗМЕРЕНИЯ",
    "Доступ к настройкам измерений - курсорных и автоматических",
    PageCursorsMeasures::self,
    PageAutoMeasures::self,
    PageMath::self,
    PageName::Measures, nullptr, Item::Active, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)
