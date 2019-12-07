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
    Tester::Polarity::Ref(), &PageTester::self, Item::Active, OnChanged_Polarity, Choice::AfterDraw
)


DEF_CHOICE_2( cControl,                                                                                                                               //--- ТЕСТЕР-КОМПОНЕНТ - Управление ---
    "Управление",
    "Тип испытательного воздействия",
    "Напряжение",
    "Ток",
    Tester::Control::Ref(), &PageTester::self, Item::Active, PageTester::OnChanged_Control, Choice::AfterDraw
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
    Tester::StepU::Ref(), &PageTester::self, Item::Active, OnChanged_Step, Choice::AfterDraw
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
    Device::SetMode(enter ? Device::Mode::Tester : Device::Mode::Osci);
}


DEF_PAGE_5_VAR( pTester, 
    "ТЕСТЕР",
    "",
    &cControl,
    &cStepU,
    &cPolarity,
    &cViewMode,
    &cAveraging,
    PageName::Tester, &PageFunction::self, Item::Active, Page::NormalTitle, OnOpenClose_Tester, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageTester::self = static_cast<const Page *>(&pTester);


void PageTester::OnChanged_Control(bool)
{
    Page *page = const_cast<Page *>(&pTester);

    Item **items = const_cast<Item **>(page->OwnData()->items);

    if (Tester::Control::IsVoltage())
    {
        items[1] = const_cast<Choice *>(&cStepU);
    }
    else
    {
        items[1] = const_cast<Choice *>(&cStepI);
    }

    Tester::LoadStep();
}
