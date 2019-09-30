#include "defines.h"
#include "Tester/Tester.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"



static void OnChanged_Polarity(bool)
{
    Tester::LoadPolarity();
}


DEF_CHOICE_2( cPolarity,                                                                                                                              //--- ТЕСТЕР-КОМПОНЕНТ - Полярность ---
    "Полярность",
    "Полярность испытательного воздействия",
    "+",
    "-",
    set.test.polarity, &PageTester::self, Item::Active, OnChanged_Polarity, Choice::AfterDraw
)


DEF_CHOICE_2( cControl,                                                                                                                               //--- ТЕСТЕР-КОМПОНЕНТ - Управление ---
    "Управление",
    "Тип испытательного воздействия",
    "Напряжение",
    "Ток",
    set.test.control, &PageTester::self, Item::Active, PageTester::OnChanged_Control, Choice::AfterDraw
)


static void OnChanged_Step(bool)
{
    Tester::LoadStep();
}

DEF_CHOICE_2( cStepU,                                                                                                                                        //--- ТЕСТЕР-КОМПОНЕНТ - Шаг ---
    "Шаг",
    "Шаг испытательного напряжения",
    "600 мВ",
    "3 В",
    set.test.stepU, &PageTester::self, Item::Active, OnChanged_Step, Choice::AfterDraw
)


DEF_CHOICE_2( cStepI,
    "Шаг",
    "Шаг исптытательного тока",
    "4 мкА",
    "20 мкА",
    set.test.stepI, &PageTester::self, Item::Active, OnChanged_Step, Choice::AfterDraw
)


DEF_CHOICE_2( cViewMode,
    "Рисование",
    "",
    "Линии",
    "Точки",
    set.test.viewMode, &PageTester::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


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
    set.test.ave, &PageTester::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


void PageTester::Init()
{
    OnChanged_Control(true);
}

static void OnOpenClose_Tester(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Tester : Device::Mode::Osci);
}


DEF_PAGE_5_VAR( pTester, // -V641
    "ТЕСТЕР",
    "",
    &cControl,
    &cStepU,
    &cPolarity,
    &cViewMode,
    &cAveraging,
    PageName::Function_Tester, &PageFunction::self, Item::Active, OnOpenClose_Tester, Page::BeforeDraw, Page::Handler
)

const Page * const PageTester::self = (const Page *)&pTester;


void PageTester::OnChanged_Control(bool)
{
    Page *page = (Page *)&pTester;

    Item **items = (Item **)page->OwnData()->items;

    if (set.test.control == TesterControl::Voltage)
    {
        items[1] = (Item *)&cStepU; //-V641
    }
    else
    {
        items[1] = (Item *)&cStepI; //-V641
    }

    Tester::LoadStep();
}
