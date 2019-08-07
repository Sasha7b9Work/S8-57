#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFunction.h"


extern const PageBase pageRecorderCursors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Center()
{

}

DEF_BUTTON( bCenter,
    "В центр",
    "",
    pageRecorderCursors, 0, OnPress_Center, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Left()
{

}

DEF_BUTTON( bLeft,
    "Лево",
    "",
    pageRecorderCursors, 0, OnPress_Left, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Right()
{

}

DEF_BUTTON( bRight,
    "Право",
    "",
    pageRecorderCursors, 0, OnPress_Right, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pageRecorderCursors, // -V641 // -V1027
    "КУРСОРЫ",
    "",
    &bCenter,
    &bLeft,
    &bRight,
    Page::Name::Function_Recorder_Show_View_Cursors, PageFunction::PageRecorder::PageShow::PageView::pointer, 0, 0, 0, 0
)

const PageBase *PageFunction::PageRecorder::PageShow::PageView::PageCursors::pointer = &pageRecorderCursors;
