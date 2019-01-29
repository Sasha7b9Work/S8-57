#include "defines.h"
#include "Tester/Tester.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"

#include "Tester/Tester_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageTester;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_Polarity(bool)
{
    Tester::LoadPolarity();
}


DEF_CHOICE_2(   cPolarity,                                                                                                                            //--- ТЕСТЕР-КОМПОНЕНТ - Полярность ---
    "Полярность", "Polarity",
    "Полярность испытательного воздействия",
    "Polarity of the test exposure",
    "+", "+",
    "-", "-",
    TESTER_POLARITY, pageTester, FuncActive, OnChanged_Polarity, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cControl,                                                                                                                             //--- ТЕСТЕР-КОМПОНЕНТ - Управление ---
    "Управление", "Control",
    "Тип испытательного воздействия",
    "Type of test exposure",
    "Напряжение", "Voltage",
    "Ток", "Current",
    TESTER_CONTROL, pageTester, FuncActive, PageFunction::PageTester::OnChanged_Control, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Step(bool)
{
    Tester::LoadStep();
}

DEF_CHOICE_2(   cStepU,                                                                                                                                      //--- ТЕСТЕР-КОМПОНЕНТ - Шаг ---
    "Шаг", "Step",
    "Шаг испытательного напряжения",
    "Test voltage step",
    "100 мВ", "100 mV",
    "500 мВ", "500 mV",
    TESTER_STEP_U, pageTester, FuncActive, OnChanged_Step, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cStepI,
    "Шаг", "Step",
    "Шаг исптытательного тока",
    "Step test current",
    "4 мА",  "4 mA",
    "20 мА", "20 mA",
    TESTER_STEP_I, pageTester, FuncActive, OnChanged_Step, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cViewMode,
    "Рисование", "Paint",
    "", "",
    "Линии", "Lines",
    "Точки", "Points",
    TESTER_VIEW_MODE, pageTester, FuncActive, Choice::FuncChange, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_5(   cSmoothing,
    "Сглаживание", "Smoothing",
    "", "",
    "1", "1",
    "2", "2",
    "3", "3",
    "4", "4",
    "5", "5",
    TESTER_NUM_SMOOTH, pageTester, FuncActive, Choice::FuncChange, FuncDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const PageBase *PageFunction::PageTester::pointer = &pageTester;

void PageFunction::PageTester::Init()
{
    OnChanged_Control(true);
}

static void OnEnterExit_Tester(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Tester : Device::Mode::Osci);
}


DEF_PAGE_4( pageTester, // -V641
    "ТЕСТЕР", "TESTER",
    "", "",
    &cControl,
    &cStepU,
    &cPolarity,
    &cViewMode,
    //&cSmoothing,
    Page::Name::Function_Tester, PageFunction::pointer, FuncActive, OnEnterExit_Tester, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageFunction::PageTester::OnChanged_Control(bool)
{
    PageBase *page = (PageBase *)&pageTester;

    Control **items = (Control **)page->items;

    if (TESTER_CONTROL_IS_U)
    {
        items[1] = (Control *)&cStepU; //-V641
    }
    else
    {
        items[1] = (Control *)&cStepI; //-V641
    }

    Tester::LoadStep();
}
