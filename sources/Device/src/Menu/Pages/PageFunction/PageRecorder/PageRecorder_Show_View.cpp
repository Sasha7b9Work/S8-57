#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Recorder/Recorder.h"
#include "Settings/Settings.h"



DEF_CHOICE_2( cMove,                                                                                                      //--- тсмйжхъ - пецхярпюрнп - опнялнрп - опнялнрп - оЕПЕЛЕЫЕМХЕ ---
    "оЕПЕЛЕЫЕМХЕ",
    "",
    "X",
    "Y",
    set.rec.axisMove, &PageRecorder::PageShow::PageView::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cZoom,                                                                                                          //--- тсмйжхъ - пецхярпюрнп - опнялнрп - опнялнрп - лЮЯЬРЮА ---
    "лЮЯЬРЮА",
    "",
    "X",
    "Y",
    set.rec.axisZoom, &PageRecorder::PageShow::PageView::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static void OnPress_Less()
{

}

DEF_BUTTON( bLess,                                                                                                             //--- тсмйжхъ - пецхярпюрнп - опнялнрп - опнялнрп - лЕМЭЬЕ ---
    "лЕМЭЬЕ",
    "",
    &PageRecorder::PageShow::PageView::self, Item::Active, OnPress_Less
)


static void OnPress_More()
{

}

DEF_BUTTON( bMore,                                                                                                             //--- тсмйжхъ - пецхярпюрнп - опнялнрп - опнялнрп - аНКЭЬЕ ---
    "аНКЭЬЕ",
    "",
    &PageRecorder::PageShow::PageView::self, Item::Active, OnPress_More
)


DEF_PAGE_5( pView, // -V641 // -V1027                                                                                                   //--- тсмйжхъ - пецхярпюрнп - опнялнрп - опнялнрп ---
    "опнялнрп",
    "",
    &cMove,                                                                 ///< тсмйжхъ - пецхярпюрнп - опнялнрп - опнялнрп - оЕПЕЛЕЫЕМХЕ
    &cZoom,                                                                 ///< тсмйжхъ - пецхярпюрнп - опнялнрп - опнялнрп - лЮЯЬРЮА
    &bLess,                                                                 ///< тсмйжхъ - пецхярпюрнп - опнялнрп - опнялнрп - лЕМЭЬЕ
    &bMore,                                                                 ///< тсмйжхъ - пецхярпюрнп - опнялнрп - опнялнрп - аНКЭЬЕ
    PageRecorder::PageShow::PageView::PageCursors::self,      ///< тсмйжхъ - пецхярпюрнп - опнялнрп - опнялнрп - йспянпш
    PageName::Function_Recorder_Show_View, &PageRecorder::PageShow::self, Item::Active, Page::Changed, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageRecorder::PageShow::PageView::self = (const Page *)&pView;
