#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Multimeter/Multimeter.h"


extern const PageDef pMultimeter;

const Page * const PageDebug::PageMultimeter::self = (const Page *)&pMultimeter;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Calibrate0()
{
    //Multimeter::Calibrate(0);
}

DEF_BUTTON( bCalibrate0,
    " ‡ÎË· 0",
    "",
    &PageDebug::PageMultimeter::self, 0, OnPress_Calibrate0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrate1()
{
    //Multimeter::Calibrate(1);
}

DEF_BUTTON(bCalibrate1,
    " ‡ÎË· 1",
    "",
    &PageDebug::PageMultimeter::self, 0, OnPress_Calibrate1, 0
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pMultimeter, // -V641 //-V1027 //-V641
    "Ã”À‹“»Ã≈“–",
    "",
    &bCalibrate0,
    &bCalibrate1,
    Page::Name::Debug_Multimeter, &PageDebug::self, 0, 0, 0, 0
)
