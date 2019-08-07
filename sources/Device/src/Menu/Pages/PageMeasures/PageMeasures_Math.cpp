#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Settings/Settings.h"
#include "Settings/SettingsMath.h"


extern const PageBase pageMath;


const PageBase *PageMeasures::PageMath::pointer = &pageMath;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cFunction,
    "Функция",
    "Выбор математической функции",
    "Сумма",
    "Произведение",
    MATH_FUNC, pageMath, Choice::EmptyActive, Choice::EmptyChange, Choice::EmptyDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_1( pageMath,
    "МАТЕМАТИКА",
    "Математические функции",
    &cFunction,
    Page::Name::Measures_Math, PageMeasures::pointer, Page::FuncActive, Page::FuncPress, Page::FuncDraw, Page::FuncRegSet
)
