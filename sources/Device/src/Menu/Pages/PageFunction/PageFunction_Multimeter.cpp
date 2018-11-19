#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageDC;
extern const PageBase pageAC;
extern const PageBase pageResistance;

const PageBase *PageFunction::PageMultimeter::pointer = &pageDC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool FuncActive_RangesVoltageDC()
{
    return MULTI_MEASURE == Multimeter::Measure::VoltageDC;
}

static void OnChange_VoltageDC(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_3(   cRangesVoltageDC,                                                                                        //--- МУЛЬТИМЕТР - Предел ---
    "Предел", "Range",
    "Диапазон измерения", "Мeasurement range",
    "2 В",   "2 V",
    "20 В",  "20 V",
    "500 В", "500 V",
    MULTI_RANGE_DC, pageDC, FuncActive_RangesVoltageDC, OnChange_VoltageDC, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool FuncActive_RnagesVoltageAC()
{
    return MULTI_MEASURE == Multimeter::Measure::VoltageAC;
}

static void OnChange_VoltageAC(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_3(   cRangesVoltageAC,                                                                                        //--- МУЛЬТИМЕТР - Предел ---
    "Предел", "Range",
    "Диапазон измерения", "Measurement range",
    "2 В",   "2 V",
    "20 В",  "20 V",
    "400 В", "400 V",
    MULTI_RANGE_AC, pageAC, FuncActive_RnagesVoltageAC, OnChange_VoltageAC, FuncDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool FuncActive_RangesReistance()
{
    return MULTI_MEASURE == Multimeter::Measure::Resistance;
}

static void OnChange_Resistance(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_4(   cRangesResistance,                                                                                       //--- Мультиметр - Предел ---
    "Предел", "Range",
    "Диапазон измерения", "Measurement range",
    "2 кОм", "2 kOhm",
    "20 кОм", "20 kOhm",
    "200 кОм", "200 kOhm",
    "10 МОм", "10 MOhm",
    MULTI_RANGE_RESISTANCE, pageResistance, FuncActive_RangesReistance, OnChange_Resistance, FuncDraw
)

DEF_CHOICE_7(   cMode,
    "Режим", "Mode"
    ,
    "Выбор типа измерениия\n"
    "U= - постоянное напряжение\n"
    "U~ - переменное напряжение\n"
    "I= - постоянный ток\n"
    "I~ - переменный ток\n"
    "R - сопротивление постоянному току\n"
    "VD - тест диодов\n"
    "BELL - прозвонка"
    ,
    "Selecting the type of measurement\n"
    "U= - DC voltage\n"
    "U~ - AC voltage\n"
    "I= - DC current\n"
    "I~ - AC current\n"
    "R - DC resistance\n"
    "VD - diodes test\n"
    "BELL - bell"
    ,
    "U=",   "U=",
    "U~",   "U~",
    "I=",   "I=",
    "I~",   "I~",
    "R",    "R",
    "VD",   "VD",
    "BELL", "BELL",
    MULTI_MEASURE, pageDC, FuncActive, PageFunction::PageMultimeter::OnChanged_Mode, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_AVP(bool)
{
    Multimeter::ChangeAVP();
}

DEF_CHOICE_2 (cAVP,
    "АВП", "ASL",
    "Автовыбор предела",
    "Auto-select limit",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    MULTI_AVP, pageDC, FuncActive, OnChanged_AVP, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageFunction::PageMultimeter::OnChanged_Mode(bool)
{
    if(MULTI_MEASURE == Multimeter::Measure::VoltageDC)
    {
        pointer = &pageDC ;
    }
    else if(MULTI_MEASURE == Multimeter::Measure::VoltageAC)
    {
        pointer = &pageAC;
    }
    else if(MULTI_MEASURE == Multimeter::Measure::Resistance)
    {
        pointer = &pageResistance;
    }

    Multimeter::ChangeMode();
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageFunction::PageMultimeter::Init()
{
    OnChanged_Mode(true);
}

static void OnPress_Page(bool enter)
{
    Device::SetMode(enter ? Device::Mode::Multimeter : Device::Mode::Osci);
}

DEF_PAGE_3( pageDC,
    "МУЛЬТИМЕТР", "MULTIMETER",
    "Управление прибором в режиме мультиметра",
    "Instrument control in multimeter mode",
    &cMode,
    &cRangesVoltageDC,
    &cAVP,
    Page::Name::Function_Multimeter, PageFunction::pointer, FuncActive, OnPress_Page, FuncDrawPage, FuncRegSetPage
)

DEF_PAGE_3(pageAC,
    "МУЛЬТИМЕТР", "MULTIMETER",
    "Управление прибором в режиме мультиметра",
    "Instrument control in multimeter mode",
    &cMode,
    &cRangesVoltageAC,
    &cAVP,
    Page::Name::Function_Multimeter, PageFunction::pointer, FuncActive, OnPress_Page, FuncDrawPage, FuncRegSetPage
)

DEF_PAGE_3(pageResistance,
    "МУЛЬТИМЕТР", "MULTIMETER",
    "Управление прибором в режиме мультиметра",
    "Instrument control in multimeter mode",
    &cMode,
    &cRangesResistance,
    &cAVP,
    Page::Name::Function_Multimeter, PageFunction::pointer, FuncActive, OnPress_Page, FuncDrawPage, FuncRegSetPage
)
