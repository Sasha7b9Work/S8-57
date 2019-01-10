#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/Sound.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageFunction;

const PageBase *PageFunction::pointer = &pageFunction;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5( pageFunction,                                                                                                            //--- ‘”Õ ÷»ﬂ ---
    "‘”Õ ÷»ﬂ", "FUNCTION",
    "¬˚·Ó ÙÛÌÍˆËË",
    "Function selection",
    PageFunction::PageFrequencyCounter::pointer,   ///< ‘”Õ ÷»ﬂ - ◊¿—“Œ“ŒÃ≈–
    PageFunction::PageFFT::pointer,                ///< ‘”Õ ÷»ﬂ - —œ≈ “–
    PageFunction::PageMultimeter::pointer,         ///< ‘”Õ ÷»ﬂ - Ã”À‹“»Ã≈“–
    PageFunction::PageTester::pointer,             ///< ‘”Õ ÷»ﬂ - “≈—“≈–
    PageFunction::PageRecorder::pointer,           ///< ‘”Õ ÷»ﬂ - –≈√»—“–¿“Œ–
    Page::Name::Function, Menu::pageMain, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
