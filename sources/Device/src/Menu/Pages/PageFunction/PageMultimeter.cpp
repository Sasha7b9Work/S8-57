#include "defines.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"


static void OnPress_Exit()
{
    Menu::CloseOpenedItem();
}


DEF_BUTTON( bExit,
    "Выход",
    "",
    &PageMultimeter::self, Item::Active, OnPress_Exit
)


static bool IsActive_RangesVoltageDC()
{
    return (set.mult.meas == MultimeterMeasure::VoltageDC);
}

static void OnChange_VoltageDC(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_3( cRangesVoltageDC,                                                                                                                                 //--- МУЛЬТИМЕТР - Предел ---
    "Диапазон",
    "Диапазон измерения",
    "2 В",
    "20 В",
    "500 В",
    Multimeter::RangeDC::Ref(), &PageMultimeter::self, IsActive_RangesVoltageDC, OnChange_VoltageDC, Choice::AfterDraw
)


static bool IsActive_RnagesVoltageAC()
{
    return (set.mult.meas == MultimeterMeasure::VoltageAC);
}

static void OnChange_VoltageAC(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_3( cRangesVoltageAC,                                                                                                                                 //--- МУЛЬТИМЕТР - Предел ---
    "Диапазон",
    "Диапазон измерения",
    "2 В",
    "20 В",
    "400 В",
    Multimeter::RangeAC::Ref(), &PageMultimeter::self, IsActive_RnagesVoltageAC, OnChange_VoltageAC, Choice::AfterDraw
)



static void OnChange_CurrentDC(bool active)
{
    if (active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_2( cRangesCurrentDC,
    "Диапазон",
    "Диапазон измерения",
    "20мА",
    "2А",
    Multimeter::RangeCurrentDC::Ref(), &PageMultimeter::self, Item::Active, OnChange_CurrentDC, Choice::AfterDraw
)


static void OnChange_CurrentAC(bool active)
{
    if (active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_2( cRangesCurrentAC,
    "Диапазон",
    "Диапазон измерения",
    "20мА",
    "2А",
    Multimeter::RangeCurrentAC::Ref(), &PageMultimeter::self, Item::Active, OnChange_CurrentAC, Choice::AfterDraw
)


static bool IsActive_RangesReistance()
{
    return (set.mult.meas == MultimeterMeasure::Resistance);
}

static void OnChange_Resistance(bool active)
{
    if(active)
    {
        Multimeter::ChangeMode();
    }
}

DEF_CHOICE_4( cRangesResistance,                                                                                                                                //--- Мультиметр - Предел ---
    "Диапазон",
    "Диапазон измерения",
    "2 кОм",
    "20 кОм",
    "200 кОм",
    "10 МОм",
    Multimeter::RangeResistance::Ref(), &PageMultimeter::self, IsActive_RangesReistance, OnChange_Resistance, Choice::AfterDraw
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
    set.mult.meas, &PageMultimeter::self, Item::Active, PageMultimeter::OnChanged_Mode, Choice::AfterDraw
)


static void OnChanged_AVP(bool)
{
    Multimeter::ChangeAVP();
}

DEF_CHOICE_2 ( cAVP,
    "АВП",
    "Автовыбор предела",
    DISABLE_RU,
    ENABLE_RU,
    set.mult.avp, &PageMultimeter::self, Item::Active, OnChanged_AVP, Choice::AfterDraw
)

static int8 zero = 0;

static void OnChanged_Zero(bool)
{
    Multimeter::LoadZero(zero);
}

DEF_CHOICE_2( cZero,
    "Нуль",
    "",
    DISABLE_RU,
    ENABLE_RU,
    zero, &PageMultimeter::self, Item::Active, OnChanged_Zero, Choice::AfterDraw
)


void PageMultimeter::Init()
{
    OnChanged_Mode(true);
}

void PageMultimeter::OnChanged_Mode(bool)
{
    Page *page = const_cast<Page *>(PageMultimeter::self);

    Item **items = const_cast<Item **>(page->OwnData()->items);

    items[1] = const_cast<Choice *>(&cRangesVoltageDC);
    items[2] = const_cast<Choice *>(&cAVP);
    items[3] = const_cast<Choice *>(&cZero);
    items[4] = const_cast<Button* >(&bExit);

    switch(set.mult.meas)
    {
    case MultimeterMeasure::VoltageDC:      items[1] = const_cast<Choice *>(&cRangesVoltageDC);       break;
    case MultimeterMeasure::VoltageAC:      items[1] = const_cast<Choice *>(&cRangesVoltageAC);       break;
    case MultimeterMeasure::CurrentDC:      items[1] = const_cast<Choice *>(&cRangesCurrentDC);       break;
    case MultimeterMeasure::CurrentAC:      items[1] = const_cast<Choice *>(&cRangesCurrentAC);       break;
    case MultimeterMeasure::Resistance:     items[1] = const_cast<Choice *>(&cRangesResistance);      break;
    case MultimeterMeasure::TestDiode:  
    case MultimeterMeasure::Bell:           items[1] = &Item::empty;
                                            items[2] = &Item::empty;
                                            items[3] = &Item::empty;                                  break;

    case MultimeterMeasure::Count:
        break;
    }

    Multimeter::ChangeMode();
}

static void OnOpenClose_Multimeter(bool enter)
{
    Device::SetMode(enter ? Device::Mode::Multimeter : Device::Mode::Osci);
}

DEF_PAGE_5_VAR( pMultimeter,
    "МУЛЬТИМЕТР",
    "Управление прибором в режиме мультиметра",
    &cMode,
    &cRangesVoltageDC,
    &cAVP,
    &cZero,
    &bExit,
    PageName::Multimeter, &PageFunction::self, Item::Active, Page::NormalTitle, OnOpenClose_Multimeter, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageMultimeter::self = static_cast<const Page *>(&pMultimeter);


static void OnPress_Calibrate0()
{
    Multimeter::Calibrate(0);
}

DEF_BUTTON( bCalibrate0,
    "Калибр 0",
    "",
    &PageMultimeter::Calibration::self, Item::Active, OnPress_Calibrate0
)


static void OnPress_Calibrate1()
{
    Multimeter::Calibrate(1);
}

DEF_BUTTON( bCalibrate1,
    "Калибр 1",
    "",
    &PageMultimeter::Calibration::self, Item::Active, OnPress_Calibrate1
)

DEF_PAGE_2( pCalibration,
    "КАЛИБРОВКА",
    "Калибровка мультиметра",
    &bCalibrate0,
    &bCalibrate1,
    PageName::Multimeter_Cal, &PageMultimeter::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageMultimeter::Calibration::self = static_cast<const Page *>(&pCalibration);


void PageMultimeter::DecodePassword(const KeyEvent &event)
{
#define NUM_SYMBOLS 10
    // Пароль
    static const Key::E password[NUM_SYMBOLS] =
    {
        Key::Function, Key::Function,
        Key::Measure,  Key::Measure,
        Key::Memory,   Key::Memory,
        Key::Display,  Key::Display,
        Key::Service,  Key::Service
    };
    // Число совпавших символов
    static int charsMatch = 0;


    if (!Device::InModeMultimeter())         // Декодирование производим только в режиме мультиметра
    {
        charsMatch = 0;
        return;
    }

    if (Menu::OpenedItem() != const_cast<Choice *>(&cMode))       // И обязательно при раскрытом меню "Режим"
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
            Page *page = const_cast<Page *>(&pMultimeter);

            Item **items = const_cast<Item **>(page->OwnData()->items);

            items[4] = const_cast<Page *>(PageMultimeter::Calibration::self);

            Menu::CloseOpenedItem();

            DISPLAY_SHOW_WARNING("Доступ к калибровке открыт");
        }
    }
    else
    {
        charsMatch = 0;
    }
}
