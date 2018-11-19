#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Tester/Tester.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pTesterU;
extern const PageBase pTesterI;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnChanged_Polarity(bool)
{
    Tester::LoadPolarity();
}


DEF_CHOICE_2(   cPolarity,                                                                                     //--- ТЕСТЕР-КОМПОНЕНТ - Полярность ---
    "Полярность", "Polarity",
    "Полярность испытательного воздействия",
    "Polarity of the test exposure",
    "+", "+",
    "-", "-",
    TESTER_POLARITY, pTesterU, FuncActive, OnChanged_Polarity, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageFunction::PageTester::OnChanged_Control(bool)
{
    if(TESTER_CONTROL_IS_U)
    {
        pointer = &pTesterU;
    }
    else
    {
        pointer = &pTesterI;
    }

    Tester::LoadStep();
}


DEF_CHOICE_2(   cControl,                                                                                      //--- ТЕСТЕР-КОМПОНЕНТ - Управление ---
    "Управление", "Control",
    "Тип испытательного воздействия",
    "Type of test exposure",
    "Напряжение", "Voltage",
    "Ток", "Current",
    TESTER_CONTROL, pTesterU, FuncActive, PageFunction::PageTester::OnChanged_Control, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Step(bool)
{
    Tester::LoadStep();
}

DEF_CHOICE_2(   cStepU,                                                                                                 //--- ТЕСТЕР-КОМПОНЕНТ - Шаг ---
    "Шаг", "Step",
    "Шаг испытательного напряжения",
    "Test voltage step",
    "100 мВ", "100 mV",
    "500 мВ", "500 mV",
    TESTER_STEP_U, pTesterU, FuncActive, OnChanged_Step, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cStepI,
    "Шаг", "Step",
    "Шаг исптытательного тока",
    "Step test current",
    "4 мА",  "4 mA",
    "20 мА", "20 mA",
    TESTER_STEP_I, pTesterI, FuncActive, OnChanged_Step, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(   cViewMode,
    "Рисование", "Paint",
    "", "",
    "Линии", "Lines",
    "Точки", "Points",
    TESTER_VIEW_MODE, pTesterU, FuncActive, FuncChangedChoice, FuncDraw
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
    TESTER_NUM_SMOOTH, pTesterU, FuncActive, FuncChangedChoice, FuncDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const PageBase *PageFunction::PageTester::pointer = &pTesterU;

void PageFunction::PageTester::Init()
{
    OnChanged_Control(true);
}

static void OnEnterExit_Tester(bool enter)
{
    Device::SetMode(enter ? Device::Mode::Tester : Device::Mode::Osci);
}


DEF_PAGE_5( pTesterU,
    "ТЕСТЕР", "TESTER",
    "", "",
    &cControl,
    &cStepU,
    &cPolarity,
    &cViewMode,
    &cSmoothing,
    Page::Name::Function_Tester, PageFunction::pointer, FuncActive, OnEnterExit_Tester, FuncDrawPage, FuncRegSetPage
)

DEF_PAGE_5(pTesterI,
    "ТЕСТЕР", "TESTER",
    "", "",
    &cControl,
    &cStepI,
    &cPolarity,
    &cViewMode,
    &cSmoothing,
    Page::Name::Function_Tester, PageFunction::pointer, FuncActive, OnEnterExit_Tester, FuncDrawPage, FuncRegSetPage
)
