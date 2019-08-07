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
    pageRecorderCursors, Button::FuncActive, OnPress_Center, Button::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Left()
{

}

DEF_BUTTON( bLeft,
    "Лево",
    "",
    pageRecorderCursors, Button::FuncActive, OnPress_Left, Button::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Right()
{

}

DEF_BUTTON( bRight,
    "Право",
    "",
    pageRecorderCursors, Button::FuncActive, OnPress_Right, Button::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pageRecorderCursors, // -V641 // -V1027
    "КУРСОРЫ",
    "",
    &bCenter,
    &bLeft,
    &bRight,
    Page::Name::Function_Recorder_Show_View_Cursors, PageFunction::PageRecorder::PageShow::PageView::pointer, Page::FuncActive, FuncPressPage, Page::FuncDraw, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageShow::PageView::PageCursors::pointer = &pageRecorderCursors;
