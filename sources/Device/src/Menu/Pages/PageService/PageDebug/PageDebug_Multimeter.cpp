#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Multimeter/Multimeter.h"


extern const Page pMultimeter;

const Page * const PageDebug::PageMultimeter::self = (const Page *)&pMultimeter;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Calibrate0()
{
    //Multimeter::Calibrate(0);
}

DEF_BUTTON( bCalibrate0,
    " ‡ÎË· 0",
    "",
    &PageDebug::PageMultimeter::self, E_BtV, OnPress_Calibrate0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrate1()
{
    //Multimeter::Calibrate(1);
}

DEF_BUTTON(bCalibrate1,
    " ‡ÎË· 1",
    "",
    &PageDebug::PageMultimeter::self, E_BtV, OnPress_Calibrate1
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pMultimeter, // -V641 //-V1027 //-V641
    "Ã”À‹“»Ã≈“–",
    "",
    &bCalibrate0,
    &bCalibrate1,
    PageName::Debug_Multimeter, &PageDebug::self, E_BtV, E_VB, E_VV, E_BfKE
)
