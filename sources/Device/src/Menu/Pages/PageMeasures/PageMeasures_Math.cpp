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
    MATH_FUNC, pageMath, Choice::FuncActive, Choice::FuncChange, Choice::FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_1( pageMath, //-V641 //-V1027
    "МАТЕМАТИКА",
    "Математические функции",
    &cFunction,
    Page::Name::Measures_Math, PageMeasures::pointer, 0, Page::FuncPress, Page::FuncDraw, Page::FuncRegSet
)
