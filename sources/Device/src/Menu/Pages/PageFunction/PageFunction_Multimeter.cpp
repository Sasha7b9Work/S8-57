#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageMultimeter;

const PageBase *PageFunction::PageMultimeter::pointer = &pageMultimeter;


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

DEF_CHOICE_3( cRangesVoltageDC,                                                                                                                                 //--- МУЛЬТИМЕТР - Предел ---
    "Предел",
    "Диапазон измерения",
    "2 В",
    "20 В",
    "500 В",
    MULTI_RANGE_VOLTAGE_DC, pageMultimeter, FuncActive_RangesVoltageDC, OnChange_VoltageDC, Choice::FuncDraw
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

DEF_CHOICE_3( cRangesVoltageAC,                                                                                                                                 //--- МУЛЬТИМЕТР - Предел ---
    "Предел",
    "Диапазон измерения",
    "2 В",
    "20 В",
    "400 В",
    MULTI_RANGE_VOLTAGE_AC, pageMultimeter, FuncActive_RnagesVoltageAC, OnChange_VoltageAC, Choice::FuncDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_CurrentDC(bool active)
{
    if (active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_2( cRangesCurrentDC,
    "Предел",
    "Диапазон измерения",
    "20мА",
    "2А",
    MULTI_RANGE_CURRENT_DC, pageMultimeter, 0, OnChange_CurrentDC, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChange_CurrentAC(bool active)
{
    if (active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_2( cRangesCurrentAC,
    "Предел",
    "Диапазон измерения",
    "20мА",
    "2А",
    MULTI_RANGE_CURRENT_AC, pageMultimeter, 0, OnChange_CurrentAC, Choice::FuncDraw
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

DEF_CHOICE_4(   cRangesResistance,                                                                                                                              //--- Мультиметр - Предел ---
    "Предел",
    "Диапазон измерения",
    "2 кОм",
    "20 кОм",
    "200 кОм",
    "10 МОм",
    MULTI_RANGE_RESISTANCE, pageMultimeter, FuncActive_RangesReistance, OnChange_Resistance, Choice::FuncDraw
)

DEF_CHOICE_7(   cMode,
    "Режим"
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
    "U=",
    "U~",
    "I=",
    "I~",
    "R",
    "VD",
    "Прозвонка",
    MULTI_MEASURE, pageMultimeter, 0, PageFunction::PageMultimeter::OnChanged_Mode, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_AVP(bool)
{
    Multimeter::ChangeAVP();
}

DEF_CHOICE_2 (cAVP,
    "АВП",
    "Автовыбор предела",
    DISABLE_RU,
    ENABLE_RU,
    MULTI_AVP, pageMultimeter, 0, OnChanged_AVP, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Zero(bool)
{
    //Multimeter::LoadZero();
}

static int8 zero = 0;

DEF_CHOICE_2(cZero,
    "Нуль",
    "",
    DISABLE_RU,
    ENABLE_RU,
    zero, pageMultimeter, 0, OnChanged_Zero, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrate0()
{
    Multimeter::Calibrate(0);
}

DEF_BUTTON( bCalibrate0,
    "Калибр 0",
    "",
    pageMultimeter, 0, OnPress_Calibrate0, Button::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrate1()
{
    Multimeter::Calibrate(1);
}

DEF_BUTTON( bCalibrate1,
    "Калибр 1",
    "",
    pageMultimeter, 0, OnPress_Calibrate1, Button::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int8 calibration = 0;

static void OnChanged_Calibration(bool)
{
    PageBase *page = (PageBase *)&pageMultimeter;

    Control **items = (Control **)page->items;

    if (calibration == 0)
    {
        items[2] = (Control *)&cAVP; //-V641
        items[3] = (Control *)&cZero; //-V641
    }
    else
    {
        items[2] = (Control *)&bCalibrate0; //-V641 //-V1027
        items[3] = (Control *)&bCalibrate1; //-V641 //-V1027
    }
}

DEF_CHOICE_2( cCalibration,
    "Калибровка",
    "",
    DISABLE_RU,
    ENABLE_RU,
    calibration, pageMultimeter, 0, OnChanged_Calibration, Choice::FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageFunction::PageMultimeter::Init()
{
    OnChanged_Mode(true);
}

static void OnPress_Page(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Multimeter : Device::Mode::Osci);
}

DEF_PAGE_5( pageMultimeter, // -V641
    "МУЛЬТИМЕТР",
    "Управление прибором в режиме мультиметра",
    &cMode,
    &cRangesVoltageDC,
    &cAVP,
    &cZero,
    &cCalibration,
    Page::Name::Function_Multimeter, PageFunction::pointer, 0, OnPress_Page, Page::FuncDraw, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageFunction::PageMultimeter::OnChanged_Mode(bool)
{
    PageBase *page = (PageBase *)&pageMultimeter;

    Control **items = (Control **)page->items;

    if (MULTI_MEASURE == Multimeter::Measure::VoltageDC)
    {
        items[1] = (Control *)&cRangesVoltageDC; //-V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::VoltageAC)
    {
        items[1] = (Control *)&cRangesVoltageAC; //-V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::CurrentDC)
    {
        items[1] = (Control *)&cRangesCurrentDC;  // -V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::CurrentAC)
    {
        items[1] = (Control *)&cRangesCurrentAC;    // -V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::Resistance)
    {
        items[1] = (Control *)&cRangesResistance; //-V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::TestDiode)
    {
        items[1] = nullptr;
    }
    else
    {
        // здесь ничего
    }

    Multimeter::ChangeMode();
}

//DEF_PAGE_1
