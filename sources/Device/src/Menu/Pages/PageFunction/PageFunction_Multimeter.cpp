#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_RangesVoltageDC()
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
    MULTI_RANGE_VOLTAGE_DC, &PageMultimeter::self, IsActive_RangesVoltageDC, OnChange_VoltageDC, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_RnagesVoltageAC()
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
    MULTI_RANGE_VOLTAGE_AC, &PageMultimeter::self, IsActive_RnagesVoltageAC, OnChange_VoltageAC, Choice::AfterDraw
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
    MULTI_RANGE_CURRENT_DC, &PageMultimeter::self, Item::Active, OnChange_CurrentDC, Choice::AfterDraw
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
    MULTI_RANGE_CURRENT_AC, &PageMultimeter::self, Item::Active, OnChange_CurrentAC, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_RangesReistance()
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

DEF_CHOICE_4( cRangesResistance,                                                                                                                                //--- Мультиметр - Предел ---
    "Предел",
    "Диапазон измерения",
    "2 кОм",
    "20 кОм",
    "200 кОм",
    "10 МОм",
    MULTI_RANGE_RESISTANCE, &PageMultimeter::self, IsActive_RangesReistance, OnChange_Resistance, Choice::AfterDraw
)

DEF_CHOICE_7( cMode,
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
    MULTI_MEASURE, &PageMultimeter::self, Item::Active, PageMultimeter::OnChanged_Mode, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_AVP(bool)
{
    Multimeter::ChangeAVP();
}

DEF_CHOICE_2 ( cAVP,
    "АВП",
    "Автовыбор предела",
    DISABLE_RU,
    ENABLE_RU,
    MULTI_AVP, &PageMultimeter::self, Item::Active, OnChanged_AVP, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Zero(bool)
{
    //Multimeter::LoadZero();
}

static int8 zero = 0;

DEF_CHOICE_2( cZero,
    "Нуль",
    "",
    DISABLE_RU,
    ENABLE_RU,
    zero, &PageMultimeter::self, Item::Active, OnChanged_Zero, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMultimeter::Init()
{
    OnChanged_Mode(true);
}

void PageMultimeter::OnChanged_Mode(bool)
{
    Page *page = (Page *)PageMultimeter::self;

    Item **items = (Item **)page->OwnData()->items;

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

static void OnOpenClose_Multimeter(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Multimeter : Device::Mode::Osci);
}

DEF_PAGE_5_VAR( pMultimeter, // -V641 //-V1027 //-V641
    "МУЛЬТИМЕТР",
    "Управление прибором в режиме мультиметра",
    &cMode,
    &cRangesVoltageDC,
    &cAVP,
    &cZero,
    &Item::empty,
    PageName::Function_Multimeter, &PageFunction::self, Item::Active, OnOpenClose_Multimeter, Page::BeforeDraw, E_BfKE
)

const Page * const PageMultimeter::self = (const Page *)&pMultimeter;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrate0()
{
    Multimeter::Calibrate(0);
}

DEF_BUTTON( bCalibrate0,
    "Калибр 0",
    "",
    &PageMultimeter::PageCalibration::self, Item::Active, OnPress_Calibrate0
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrate1()
{
    Multimeter::Calibrate(1);
}

DEF_BUTTON( bCalibrate1,
    "Калибр 1",
    "",
    &PageMultimeter::PageCalibration::self, Item::Active, OnPress_Calibrate1
)

DEF_PAGE_2( pCalibration, //-V641 //-V1027
    "КАЛИБРОВКА",
    "Калибровка мультиметра",
    &bCalibrate0,
    &bCalibrate1,
    PageName::Function_Multimeter_Cal, &PageMultimeter::self, Item::Active, Choice::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageMultimeter::PageCalibration::self = (const Page *)&pCalibration;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageMultimeter::DecodePassword(const KeyEvent &event)
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
            Page *page = (Page *)&pMultimeter;

            Item **items = (Item **)page->OwnData()->items;

            items[4] = (Item *)PageMultimeter::PageCalibration::self;

            Menu::CloseOpenedItem();
        }
    }
    else
    {
        charsMatch = 0;
    }
}
