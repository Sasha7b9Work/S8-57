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


DEF_CHOICE_2( cPolarity,                                                                                                                              //--- ТЕСТЕР-КОМПОНЕНТ - Полярность ---
    "Полярность",
    "Полярность испытательного воздействия",
    "+",
    "-",
    TESTER_POLARITY, pageTester, FuncActive, OnChanged_Polarity, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cControl,                                                                                                                             //--- ТЕСТЕР-КОМПОНЕНТ - Управление ---
    "Управление",
    "Тип испытательного воздействия",
    "Напряжение",
    "Ток",
    TESTER_CONTROL, pageTester, FuncActive, PageFunction::PageTester::OnChanged_Control, Choice::FuncDraw
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
    TESTER_STEP_U, pageTester, FuncActive, OnChanged_Step, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cStepI,
    "Шаг",
    "Шаг исптытательного тока",
    "4 мкА",
    "20 мкА",
    TESTER_STEP_I, pageTester, FuncActive, OnChanged_Step, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cViewMode,
    "Рисование",
    "",
    "Линии",
    "Точки",
    TESTER_VIEW_MODE, pageTester, FuncActive, Choice::FuncChange, Choice::FuncDraw
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
    TESTER_ENUM_AVERAGE, pageTester, FuncActive, Choice::FuncChange, Choice::FuncDraw
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


DEF_PAGE_5( pageTester, // -V641
    "ТЕСТЕР",
    "",
    &cControl,
    &cStepU,
    &cPolarity,
    &cViewMode,
    &cAveraging,
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
