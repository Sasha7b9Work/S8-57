// 2023/11/21 10:25:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Tester/Tester.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Device.h"


namespace PageTester
{
    static void OnPress_Exit()
    {
        Menu::CloseOpenedItem();
    }


    DEF_BUTTON(bExit,
        "Выход",
        "",
        &PageTester::self, Item::Active, OnPress_Exit
    )


        static void OnChanged_Polarity(bool)
    {
        Tester::LoadPolarity();
    }

    DEF_CHOICE_2(cPolarity,                                                                                                                                        //--- ТЕСТЕР - Полярность ---
        "Полярность",
        "Полярность испытательного воздействия",
        "+",
        "-",
        S_TEST_POLARITY, &PageTester::self, Item::Active, OnChanged_Polarity, Choice::AfterDraw
    )


        DEF_CHOICE_2(cConductivity,                                                                                                                                  //--- ТЕСТЕР - Проводимость ---
            "Проводимость",
            "Тип проводимости биполярного транзистора",
            "n-p-n",
            "p-n-p",
            S_TEST_POLARITY, &PageTester::self, Item::Active, OnChanged_Polarity, Choice::AfterDraw
        )


        DEF_CHOICE_2(cControl,                                                                                                                                         //--- ТЕСТЕР - Управление ---
            "Управление",
            "Тип испытательного воздействия",
            "Напряжение",
            "Ток",
            S_TEST_CONTROL, &PageTester::self, Item::Active, PageTester::OnChanged_Control, Choice::AfterDraw
        )


        static void OnChanged_Step(bool)
    {
        Tester::LoadStep();
    }

    DEF_CHOICE_2(cStepU,                                                                                                                                        //--- ТЕСТЕР-КОМПОНЕНТ - Шаг ---
        "Шаг",
        "Шаг испытательного напряжения",
        "600 мВ",
        "3 В",
        S_TEST_STEP_U, &PageTester::self, Item::Active, OnChanged_Step, Choice::AfterDraw
    )


        DEF_CHOICE_2(cStepI,
            "Шаг",
            "Шаг исптытательного тока",
            "4 мкА",
            "20 мкА",
            S_TEST_STEP_I, &PageTester::self, Item::Active, OnChanged_Step, Choice::AfterDraw
        )


        DEF_CHOICE_2(cViewMode,
            "Отображение",
            "",
            "Вектор",
            "Точки",
            S_TEST_VIEW_MODE, &PageTester::self, Item::Active, Choice::Changed, Choice::AfterDraw
        )


        DEF_CHOICE_9(cAveraging,
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
            S_TEST_ENUM_AVERAGE, &PageTester::self, Item::Active, Choice::Changed, Choice::AfterDraw
        )


        void PageTester::Init()
    {
        OnChanged_Control(true);
    }

    static void OnOpenClose_Tester(bool enter)
    {
        Device::SetMode(enter ? Device::Mode::Tester : Device::Mode::Osci);
    }


    DEF_PAGE_6_VAR(pTester,  //-V2567
        "ТЕСТЕР",
        "",
        &cControl,
        &cStepU,
        &cConductivity,
        &cViewMode,
        &bExit,
        &cAveraging,
        PageName::Tester, &PageFunction::self, Item::Active, Page::NormalTitle, OnOpenClose_Tester, Page::BeforeDraw, Page::HandlerKeyEvent
    )
}

const Page * const PageTester::self = static_cast<const Page *>(&PageTester::pTester);


void PageTester::OnChanged_Control(bool)
{
    Page *page = const_cast<Page *>(&pTester); //-V2567

    Item **items = const_cast<Item **>(page->OwnData()->items); //-V2567

    if (S_TEST_CONTROL_IS_VOLTAGE)
    {
        items[1] = const_cast<Choice *>(&cStepU); //-V2567
        items[2] = const_cast<Choice *>(&cPolarity); //-V2567
    }
    else
    {
        items[1] = const_cast<Choice *>(&cStepI); //-V2567
        items[2] = const_cast<Choice *>(&cConductivity); //-V2567
    }

    Tester::LoadStep();
}
