#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"


extern const PageDef pageMultimeter;
extern const PageDef pageCalibration;

const Page * const PageMultimeter::self = (const Page *)&pageMultimeter;
const Page * const PageMultimeter::PageCalibration::self = (const Page *)&pageCalibration;


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
    MULTI_RANGE_VOLTAGE_DC, &PageMultimeter::self, FuncActive_RangesVoltageDC, OnChange_VoltageDC, 0
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
    MULTI_RANGE_VOLTAGE_AC, &PageMultimeter::self, FuncActive_RnagesVoltageAC, OnChange_VoltageAC, 0
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
    MULTI_RANGE_CURRENT_DC, &PageMultimeter::self, 0, OnChange_CurrentDC, 0
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
    MULTI_RANGE_CURRENT_AC, &PageMultimeter::self, 0, OnChange_CurrentAC, 0
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
    MULTI_RANGE_RESISTANCE, &PageMultimeter::self, FuncActive_RangesReistance, OnChange_Resistance, 0
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
    MULTI_MEASURE, &PageMultimeter::self, 0, PageMultimeter::OnChanged_Mode, 0
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
    MULTI_AVP, &PageMultimeter::self, 0, OnChanged_AVP, 0
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
    zero, &PageMultimeter::self, 0, OnChanged_Zero, 0
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageMultimeter::Init()
{
    OnChanged_Mode(true);
}

void PageMultimeter::OnChanged_Mode(bool)
{
    Page *page = (Page *)&pageMultimeter;

    Item **items = (Item **)page->items;

    if (MULTI_MEASURE == Multimeter::Measure::VoltageDC)
    {
        items[1] = (Item *)&cRangesVoltageDC; //-V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::VoltageAC)
    {
        items[1] = (Item *)&cRangesVoltageAC; //-V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::CurrentDC)
    {
        items[1] = (Item *)&cRangesCurrentDC;  // -V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::CurrentAC)
    {
        items[1] = (Item *)&cRangesCurrentAC;    // -V641
    }
    else if (MULTI_MEASURE == Multimeter::Measure::Resistance)
    {
        items[1] = (Item *)&cRangesResistance; //-V641
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

static void OnEnterExit_Multimeter(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Multimeter : Device::Mode::Osci);
}

DEF_PAGE_5_VAR( pageMultimeter, // -V641 //-V1027 //-V641
    "МУЛЬТИМЕТР",
    "Управление прибором в режиме мультиметра",
    &cMode,
    &cRangesVoltageDC,
    &cAVP,
    &cZero,
    &Item::empty,
    Page::Name::Function_Multimeter, &PageFunction::self, 0, OnEnterExit_Multimeter, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrate0()
{
    Multimeter::Calibrate(0);
}

DEF_BUTTON( bCalibrate0,
    "Калибр 0",
    "",
    &PageMultimeter::PageCalibration::self, 0, OnPress_Calibrate0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrate1()
{
    Multimeter::Calibrate(1);
}

DEF_BUTTON( bCalibrate1,
    "Калибр 1",
    "",
    &PageMultimeter::PageCalibration::self, 0, OnPress_Calibrate1, 0
)


DEF_PAGE_2( pageCalibration, //-V641 //-V1027
    "КАЛИБРОВКА",
    "Калибровка мультиметра",
    &bCalibrate0,
    &bCalibrate1,
    Page::Name::Function_Multimeter_Cal, &PageMultimeter::self, 0, 0, 0, 0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMultimeter::DecodePassword(KeyEvent &event)
{
#define NUM_SYMBOLS 8
    /// Пароль
    static const Key::E password[NUM_SYMBOLS] = { Key::F2, Key::F2, Key::F3, Key::F3, Key::F4, Key::F4, Key::F5, Key::F5 };
    /// Число совпавших символов
    static int charsMatch = 0;


    if (!Device::State::InModeMultimeter())         // Декодирование производим только в режиме мультиметра
    {
        charsMatch = 0;
        return;
    }

    if (Menu::OpenedItem() != (Item *)&cMode)       // И обязательно при раскрытом меню "Режим" //-V641
    {
        charsMatch = 0;
        return;
    }

    if (event.type != TypePress::Release)
    {
        return;
    }

    if (password[charsMatch++] == event.key)
    {
        if (charsMatch == NUM_SYMBOLS)
        {
            Page *page = (Page *)&pageMultimeter;

            Item **items = (Item **)page->items;

            items[4] = (Item *)PageMultimeter::PageCalibration::self;

            Menu::CloseOpenedItem();
        }
    }
    else
    {
        charsMatch = 0;
    }
}
