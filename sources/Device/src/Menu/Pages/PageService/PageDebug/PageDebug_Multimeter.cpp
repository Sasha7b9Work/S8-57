#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Multimeter/Multimeter.h"


extern const PageBase pMultimeter;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageService::PageDebug::PageMultimeter::pointer = &pMultimeter;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Calibrate0()
{
    //Multimeter::Calibrate(0);
}

DEF_BUTTON( bCalibrate0,
    " ‡ÎË· 0",
    "",
    pMultimeter, 0, OnPress_Calibrate0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrate1()
{
    //Multimeter::Calibrate(1);
}

DEF_BUTTON(bCalibrate1,
    " ‡ÎË· 1",
    "",
    pMultimeter, 0, OnPress_Calibrate1, 0
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(pMultimeter, // -V641 //-V1027 //-V641
    "Ã”À‹“»Ã≈“–",
    "",
    &bCalibrate0,
    &bCalibrate1,
    Page::Name::Debug_Multimeter, PageService::PageDebug::pointer, 0, 0, 0, FuncRegSetPage
)
