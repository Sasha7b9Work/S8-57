#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageFunction.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageOperations;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Rename()
{

}

DEF_BUTTON( bRename,                                                                                                    //--- ‘”Õ ÷»ﬂ - –≈√»—“–¿“Œ– - œ–Œ—ÃŒ“– - Œœ≈–¿÷»» - œÂÂËÏÂÌÓ‚‡Ú¸ ---
    "œÂÂËÏÂÌÓ‚‡Ú¸", "Rename",
    "",
    "",
    pageOperations, FuncActive, OnPress_Rename, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Copy()
{

}

DEF_BUTTON( bCopy,                                                                                                         //--- ‘”Õ ÷»ﬂ - –≈√»—“–¿“Œ– - œ–Œ—ÃŒ“– - Œœ≈–¿÷»» -  ÓÔËÓ‚‡Ú¸ ---
    " ÓÔËÓ‚‡Ú¸", "Copy",
    "",
    "",
    pageOperations, FuncActive, OnPress_Copy, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageOperations,                                                                                                             //--- ‘”Õ ÷»ﬂ - –≈√»—“–¿“Œ– - œ–Œ—ÃŒ“– - Œœ≈–¿÷»» ---
    "Œœ≈–¿÷»»", "OPERATIONS",
    "",
    "",
    &bRename,   ///< ‘”Õ ÷»ﬂ - –≈√»—“–¿“Œ– - œ–Œ—ÃŒ“– - Œœ≈–¿÷»» - œÂÂËÏÂÌÓ‚‡Ú¸
    &bCopy,     ///< ‘”Õ ÷»ﬂ - –≈√»—“–¿“Œ– - œ–Œ—ÃŒ“– - Œœ≈–¿÷»» -  ÓÔËÓ‚‡Ú¸
    Page::Name::Function_Recorder_Show_Operations, PageFunction::PageRecorder::PageShow::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)

const PageBase *PageFunction::PageRecorder::PageShow::PageOperations::pointer = &pageOperations;
