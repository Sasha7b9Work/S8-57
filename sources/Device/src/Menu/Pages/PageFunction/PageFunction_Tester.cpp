#include "defines.h"
#include "Tester/Tester.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"
#include "Tester/Tester_Settings.h"


extern const Page pageTester;

const Page * const PageTester::self = (const Page *)&pageTester;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_Polarity(bool)
{
    Tester::LoadPolarity();
}


DEF_CHOICE_2( cPolarity,                                                                                                                              //--- ТЕСТЕР-КОМПОНЕНТ - Полярность ---
    "Полярность",
    "Полярность испытательного воздействия",
    "+",
    "-",
    TESTER_POLARITY, &PageTester::self, E_BtV, OnChanged_Polarity, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cControl,                                                                                                                             //--- ТЕСТЕР-КОМПОНЕНТ - Управление ---
    "Управление",
    "Тип испытательного воздействия",
    "Напряжение",
    "Ток",
    TESTER_CONTROL, &PageTester::self, E_BtV, PageTester::OnChanged_Control, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Step(bool)
{
    Tester::LoadStep();
}

DEF_CHOICE_2(   cStepU,                                                                                                                                      //--- ТЕСТЕР-КОМПОНЕНТ - Шаг ---
    "Шаг",
    "Шаг испытательного напряжения",
    "600 мВ",
    "3 В",
    TESTER_STEP_U, &PageTester::self, E_BtV, OnChanged_Step, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cStepI,
    "Шаг",
    "Шаг исптытательного тока",
    "4 мкА",
    "20 мкА",
    TESTER_STEP_I, &PageTester::self, E_BtV, OnChanged_Step, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cViewMode,
    "Рисование",
    "",
    "Линии",
    "Точки",
    TESTER_VIEW_MODE, &PageTester::self, E_BtV, E_VB, E_VII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_9( cAveraging,
    "Усреднение",
    "Задаёт количество усредняемых измерений, по которым будет выводиться итоговый результат. Чем выше это значение, тем более стабильным получается результат, но дольше ждать",
    "1",
    "2",
    "4",
    "8",
    "16",
    "32",
    "64",
    "128",
    "256",
    TESTER_ENUM_AVERAGE, &PageTester::self, E_BtV, E_VB, E_VII
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageTester::Init()
{
    OnChanged_Control(true);
}

static void OnOpenClose_Tester(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Tester : Device::Mode::Osci);
}


DEF_PAGE_5_VAR( pageTester, // -V641
    "ТЕСТЕР",
    "",
    &cControl,
    &cStepU,
    &cPolarity,
    &cViewMode,
    &cAveraging,
    PageName::Function_Tester, &PageFunction::self, E_BtV, OnOpenClose_Tester, E_VV, E_BfKE
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageTester::OnChanged_Control(bool)
{
    Page *page = (Page *)&pageTester;

    Item **items = (Item **)page->OwnData()->items;

    if (TESTER_CONTROL_IS_U)
    {
        items[1] = (Item *)&cStepU; //-V641
    }
    else
    {
        items[1] = (Item *)&cStepI; //-V641
    }

    Tester::LoadStep();
}
