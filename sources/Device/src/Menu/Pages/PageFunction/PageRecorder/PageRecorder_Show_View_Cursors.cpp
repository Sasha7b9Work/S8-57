#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFunction.h"


extern const Page pageRecorderCursors;

const Page * const PageRecorder::PageShow::PageView::PageCursors::self = (const Page *)&pageRecorderCursors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Center()
{

}

DEF_BUTTON( bCenter,
    "В центр",
    "",
    &PageRecorder::PageShow::PageView::PageCursors::self, E_BtV, OnPress_Center, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Left()
{

}

DEF_BUTTON( bLeft,
    "Лево",
    "",
    &PageRecorder::PageShow::PageView::PageCursors::self, E_BtV, OnPress_Left, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Right()
{

}

DEF_BUTTON( bRight,
    "Право",
    "",
    &PageRecorder::PageShow::PageView::PageCursors::self, E_BtV, OnPress_Right, E_VII
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pageRecorderCursors, // -V641 // -V1027
    "КУРСОРЫ",
    "",
    &bCenter,
    &bLeft,
    &bRight,
    PageName::Function_Recorder_Show_View_Cursors, &PageRecorder::PageShow::PageView::self, E_BtV, E_VB, E_VV, E_BfKE
)
