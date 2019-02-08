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
    "Предел", "Range",
    "Диапазон измерения", "Мeasurement range",
    "2 В",   "2 V",
    "20 В",  "20 V",
    "500 В", "500 V",
    MULTI_RANGE_DC, pageMultimeter, FuncActive_RangesVoltageDC, OnChange_VoltageDC, Choice::EmptyDraw
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
    "Предел", "Range",
    "Диапазон измерения", "Measurement range",
    "2 В", "2 V",
    "20 В", "20 V",
    "400 В", "400 V",
    MULTI_RANGE_AC, pageMultimeter, FuncActive_RnagesVoltageAC, OnChange_VoltageAC, Choice::EmptyDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

static uint8 rangeCurrent;

DEF_CHOICE_2( cRangesCurrent,
    "Предел", "Range",
    "Диапазон измерения", "Measurement range",
    "2А", "2А",
    "2А", "2А",
    rangeCurrent, pageMultimeter, EmptyFuncBfV, Choice::EmptyChange, Choice::EmptyDraw
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
    "Предел", "Range",
    "Диапазон измерения", "Measurement range",
    "2 кОм", "2 kOhm",
    "20 кОм", "20 kOhm",
    "200 кОм", "200 kOhm",
    "10 МОм", "10 MOhm",
    MULTI_RANGE_RESISTANCE, pageMultimeter, FuncActive_RangesReistance, OnChange_Resistance, Choice::EmptyDraw
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
    "Прозвонка", "Bell",
    MULTI_MEASURE, pageMultimeter, FuncActive, PageFunction::PageMultimeter::OnChanged_Mode, Choice::EmptyDraw
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
    MULTI_AVP, pageMultimeter, FuncActive, OnChanged_AVP, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Zero(bool)
{
    Multimeter::LoadZero();
}

DEF_CHOICE_2( cZero,
    "Нуль", "Zero",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU, ENABLE_EN,
    Multimeter::zero, pageMultimeter, FuncActive, OnChanged_Zero, Choice::EmptyDraw
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

DEF_PAGE_4( pageMultimeter, // -V641
    "МУЛЬТИМЕТР", "MULTIMETER",
    "Управление прибором в режиме мультиметра",
    "Instrument control in multimeter mode",
    &cMode,
    &cRangesVoltageDC,
    &cAVP,
    &cZero,
    Page::Name::Function_Multimeter, PageFunction::pointer, FuncActive, OnPress_Page, FuncDrawPage, FuncRegSetPage
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
    else if (MULTI_MEASURE == Multimeter::Measure::CurrentDC || MULTI_MEASURE == Multimeter::Measure::CurrentAC)
    {
        items[1] = (Control *)&cRangesCurrent;  // -V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::Resistance)
    {
        items[1] = (Control *)&cRangesResistance; //-V641
    }
    else
    {
        LOG_ERROR("");
    }

    Multimeter::ChangeMode();
}
