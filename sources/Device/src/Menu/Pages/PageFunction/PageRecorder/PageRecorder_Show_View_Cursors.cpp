#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageRecorder.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Center()
{

}

DEF_BUTTON( bCenter,
    "В центр",
    "",
    &PageRecorder::PageShow::PageView::PageCursors::self, Item::Active, OnPress_Center
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Left()
{

}

DEF_BUTTON( bLeft,
    "Лево",
    "",
    &PageRecorder::PageShow::PageView::PageCursors::self, Item::Active, OnPress_Left
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Right()
{

}

DEF_BUTTON( bRight,
    "Право",
    "",
    &PageRecorder::PageShow::PageView::PageCursors::self, Item::Active, OnPress_Right
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3(pCursors, // -V641 // -V1027
    "КУРСОРЫ",
    "",
    &bCenter,
    &bLeft,
    &bRight,
    PageName::Function_Recorder_Show_View_Cursors, &PageRecorder::PageShow::PageView::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageRecorder::PageShow::PageView::PageCursors::self = (const Page *)&pCursors;
