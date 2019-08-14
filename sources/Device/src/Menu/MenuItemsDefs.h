#pragma once
#include "defines.h"
#include "Utils/CommonFunctions.h"


class Page;
class Item;
struct StructHelpDrawButton;
typedef bool(*pFuncBKE)(KeyEvent);

/// Общая часть для всех типов элементов меню
#define COMMON_PART_MENU_ITEM                                                                           \
    uint8                       type;          /* Тип итема */                                                     \
    int8                        num;           /* Число вариантов для Choice или число контролов для Page*/        \
    uint8                       name;          /* Имя из перечисления Page::Name */                                \
    const Page * const         *keeper;        /* Адрес страницы, которой принадлежит. Для Page_Main = 0 */        \
    pFuncBV                     funcOfActive;  /* Активен ли данный элемент */                                     \
    const char                 *titleHint[2]   /* Название страницы на русском и английском языках. Также подсказка для режима помощи */


/// Описывает страницу меню.
struct PageDef
{
    COMMON_PART_MENU_ITEM;
    const Item * const         *items;              ///< Здесь указатели на пункты этой страницы (в обычной странице)
                                                    ///< для страницы малых кнопок  здесь хранятся 6 указателей на SButton : 0 - K_Enter, 1...5 - K_1...K_5
    pFuncVB                     funcOnEnterExit;    ///< Будет вызываться при нажатии на свёрнутую страницу и при выходе из этой страницы на предыдущую
    pFuncVV                     funcOnDraw;         ///< Будет вызываться после отрисовки кнопок
    pFuncBKE                    funcRegSet;         ///< Вызывается при нажатии кнопок ВЛЕВО/ВПРАВО. Если отработала, возвращает true
};

/// Описывает кнопку.
struct ButtonDef
{
    COMMON_PART_MENU_ITEM;
    pFuncVV                     funcOnPress;        ///< Функция, которая вызывается при нажатии на кнопку.
    pFuncVII                    funcForDraw;        ///< Функция будет вызываться во время отрисовки кнопки.
};

/// Описывает кнопку для дополнительного режима меню.
struct GraphButtonDef
{
    COMMON_PART_MENU_ITEM;
    pFuncVV                     funcOnPress;    ///< Эта функция вызвается для обработки нажатия кнопки.
    pFuncVII                    funcForDraw;    ///< Эта функция вызывается для отрисовки кнопки в месте с координатами x, y.
    const StructHelpDrawButton *hintUGO;
    int                         numHints;
};

/// Описывает регулятор.
struct GovernorDef
{
    COMMON_PART_MENU_ITEM;
    int16                      *cell;
    int16                       minValue;       ///< Минмальное значение, которое может принимать регулятор.
    int16                       maxValue;       ///< Максимальное значение.
    pFuncVV                     funcOfChanged;  ///< Функция, которую нужно вызывать после того, как значение регулятора изменилось.
    pFuncVV                     funcBeforeDraw; ///< Функция, которая вызывается перед отрисовкой
};

struct ChoiceDef
{
    COMMON_PART_MENU_ITEM;
    int8                       *cell;
    const char  * const        *names;          ///< Варианты выбора на русском и английском языках.
    pFuncVB			            funcOnChanged;  ///< Функция должна вызываться после изменения значения элемента.
    pFuncVII                    funcForDraw;    ///< Функция вызывается после отрисовки элемента. 
};

class ColorType;

struct GovernorColorDef
{
    COMMON_PART_MENU_ITEM;
    ColorType                  *ct;             ///< Структура для описания цвета.
    pFuncVV                     funcOnChanged;  ///< Эту функцию нужно вызывать после изменения значения элемента.
};

struct TimeDef
{
    COMMON_PART_MENU_ITEM;
    int8                        *curField;      ///< Текущее поле установки. 0 - выход, 1 - сек, 2 - мин, 3 - часы, 4 - день, 5 - месяц, 6 - год, 7 - установить.
    int8                        *hours;
    int8                        *minutes;
    int8                        *seconds;
    int8                        *month;
    int8                        *day;
    int8                        *year;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_BUTTON(name, title, hint, keeper, funcActive, funcPress, funcDraw)                                                                                                              \
static const ButtonDef name = { Item::Type::Button, 0, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                               \
    funcPress, funcDraw };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_TIME(name, title, hint, keeper, funcActive, cur, h, mi, s, mo, d, y)                                                                                                            \
static const TimeBase name = { Item::Type::Time, 0, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                           \
    &cur, &h, &mi, &s, &mo, &d, &y };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GOVERNOR(name, title, hint, cell, min, max, keeper, funcActive, funcChanged, funcBeforeDraw)                                                                                    \
static const GovernorDef name = {Item::Type::Governor, 0, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                            \
    &(cell), min, max, funcChanged, funcBeforeDraw};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_IP_ADDRESS(name, titleRU, titleEN, hintRU, hintEN, ip0, ip1, ip2, ip3, port, keeper, funcActive, funcOfChanged)                                                                 \
static const IPaddressBase name = {Item_IP, 0, false, Page::Name::NoPage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                                           \
    &ip0, &ip1, &ip2, &ip3, funcOfChanged, port};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_MAC_ADDRESS(name, titleRU, titleEN, hintRU, hintEN, mac0, mac1, mac2, mac3, mac4, mac5, keeper, funcActive, funcOfChanged)                                                      \
static const MACaddressBase name = {Item_MAC, 0, false, Page::Name::NoPage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                                         \
    &mac0, &mac1, &mac2, &mac3, &mac4, &mac5, funcOfChanged};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GOVERNOR_COLOR(name, title, hint, colorType, keeper)                                                                                                                            \
static const GovernorColorDef name = {Item::Type::GovernorColor, 0, Page::Name::NoPage, keeper, 0,                                                                                          \
    {title, hint}, &colorType, 0};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_PAGE_SB(name, title, hint, item0, item1, item2, item3, item4, item5, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                             \
static const Item * const  items##name[] = {(Item *)item0, (Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4,                                                                      \
    (Item *)item5};                                                                                                                                                                         \
static const PageDef name = {Item::Type::Page, 6, namePage, keeper, funcActive, {title, hint}, items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_1(name, title, hint, item1, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                                 \
static const Item * const  items##name[] = {(Item *)item1};                                                                                                                                 \
static const PageDef name = {Item::Type::Page, 1, namePage, keeper, funcActive, {title, hint},                                                                                              \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_2(name, title, hint, item1, item2, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                          \
static const Item * const  items##name[] = {(Item *)item1, (Item *)item2};                                                                                                                  \
static const PageDef name = {Item::Type::Page, 2, namePage, keeper, funcActive, {title, hint},                                                                                              \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_3(name, title, hint, item1, item2, item3, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                   \
static const Item * const items##name[] = {(Item *)(item1), (Item *)(item2), (Item *)(item3)};                                                                                              \
static const PageDef name = {Item::Type::Page, 3, namePage, keeper, funcActive, {title, hint},                                                                                              \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_4(name, title, hint, item1, item2, item3, item4, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                            \
static const Item * items##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4};                                                                                           \
static const PageDef name = {Item::Type::Page, 4, namePage, keeper, funcActive, {title, hint},                                                                                              \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_5(name, title, hint,                                                                                                                                                       \
    item1, item2, item3, item4, item5, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                                       \
static const Item * const items##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5};                                                                      \
static const PageDef name = {Item::Type::Page, 5, namePage, keeper, funcActive, {title, hint},                                                                                              \
    items##name, funcPress, funcDraw, funcRegSet};

/// Определяет страницу так, что items##name размещается в ОЗУ
#define DEF_PAGE_5_VAR(name, title, hint,                                                                                                                                                   \
    item1, item2, item3, item4, item5, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                                       \
static const Item * items##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5};                                                                            \
static const PageDef name = {Item::Type::Page, 5, namePage, keeper, funcActive, {title, hint},                                                                                              \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_6(name, title, hint, item1, item2, item3, item4, item5, item6, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                              \
static const Item * const  items##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6};                                                      \
static const PageDef name = {Item::Type::Page, 6, namePage, keeper, funcActive, {title, hint},                                                                                              \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_7(name, title, hint,                                                                                                                                                       \
    item1, item2, item3, item4, item5, item6, item7, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                         \
static const Item * const  items##name[] = {                                                                                                                                                \
    (Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6, (Item *)item7};                                                                               \
static const PageDef name = {Item::Type::Page, 7, namePage, keeper, funcActive, {title, hint},                                                                                              \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_8(name, title, hint, item1, item2, item3, item4, item5, item6, item7, item8, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                \
static const Item * const  items##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5,                                                                      \
    (Item *)item6, (Item *)item7, (Item *)item8};                                                                                                                                           \
static const PageDef name = {Item::Type::Page, 8, namePage, keeper, funcActive, {title, hint},                                                                                              \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_9(name, title, hint,                                                                                                                                                       \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                           \
static const Item * const  items##name[] = {                                                                                                                                                \
    (Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6, (Item *)item7,                                                                                \
    (Item *)item8, (Item *)item9};                                                                                                                                                          \
static const PageDef name = {Item::Type::Page, 9, false, namePage, keeper, funcActive, {title, hint},                                                                                       \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_10(name, titleRU, titleEN, hintRU, hintEN,                                                                                                                                 \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                   \
static const Item * const  items##name[] = {                                                                                                                                                \
    (Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6, (Item *)item7,                                                                                \
    (Item *)item8, (Item *)item9, (Item *)item10};                                                                                                                                          \
static const PageDef name = {Item::Type::Page, 10, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                                                 \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_11(name, titleRU, titleEN, hintRU, hintEN,                                                                                                                                 \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                           \
static const Item * const  items##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5,                                                                      \
    (Item *)item6, (Item *)item7, (Item *)item8, (Item *)item9, (Item *)item10, (Item *)item11};                                                                                            \
static const PageDef name = {Item::Type::Page, 11, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                                                 \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_12(name, title, hint,                                                                                                                                                      \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                   \
static const Item * const  items##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5,                                                                      \
    (Item *)item6, (Item *)item7, (Item *)item8, (Item *)item9, (Item *)item10, (Item *)item11, (Item *)item12};                                                                            \
static const PageDef name = {Item::Type::Page, 12, namePage, keeper, funcActive, {title, hint},                                                                                             \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_13(name, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                                                                        \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, item13)                                                                                          \
static const Item * const  items##name[] = {                                                                                                                                                \
    (Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6, (Item *)item7,                                                                                \
    (Item *)item8, (Item *)item9, (Item *)item10, (Item *)item11, (Item *)item12, (Item *)item13};                                                                                          \
static const PageDef name = {Item::Type::Page, 13, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                                                 \
    items##name, funcPress, FuncDrawPage, 0};

#define DEF_PAGE_15(name, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                                                                        \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10,                                                                                                                  \
    item11, item12, item13, item14, item15)                                                                                                                                                 \
static const Item * const  items##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5,                                                                      \
    (Item *)item6, (Item *)item7, (Item *)item8, (Item *)item9, (Item *)item10, (Item *)item11, (Item *)item12,                                                                             \
    (Item *)item13, (Item *)item14, (Item *)item15};                                                                                                                                        \
static const PageDef name = {Item::Type::Page, 15, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                                                 \
    items##name, funcPress, FuncDrawPage, 0};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_CHOICE_PARAMETER(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, funcPress, numParam,                                                                               \
    allow0, allow1, allow2, allow3, allow4, allow5, allow6, allow7)                                                                                                                         \
ChoiceParameterBase name = {Item_ChoiceParameter, 0, false, Page::Name::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN} ,                                                   \
    funcPress, {allow0, allow1, allow2, allow3, allow4, allow5, allow6, allow7}, (uint8 *)&numParam};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GRAPH_BUTTON(name, title, hint, keeper, funcActive, funcPress, funcDraw)                                                                                                        \
static const GraphButtonDef name = { Item::Type::GraphButton, 0, Page::Name::NoPage, keeper, funcActive,                                                                                    \
     {title, hint}, funcPress, funcDraw, 0, 0};

#define DEF_GRAPH_BUTTON_EXIT(name, keeper, funcActive, funcPress, funcDraw)                                                                                                                \
static const GraphButtonDef name = { Item::Type::GraphButton, 0, Page::Name::NoPage, keeper, funcActive, {"Выход",                                                                          \
    "Кнопка для выхода в предыдущее меню"}, funcPress, funcDraw, 0, 0};

#define DEF_GRAPH_BUTTON_HINTS_2(name, title, hint, keeper, funcActive, funcPress, funcDraw, FuncDrawHint1, hint1, FuncDrawHint2, hint2)                                                    \
static const StructHelpDrawButton hints##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 }};                                                                                     \
static const GraphButtonDef name = { Item::Type::GraphButton, 0, Page::Name::NoPage, keeper, funcActive,                                                                                    \
     {title, hint}, funcPress, funcDraw, hints##name, 2};

#define DEF_GRAPH_BUTTON_HINTS_3(name, title, hint, keeper, funcActive, funcPress, funcDraw,                                                                                                \
    FuncDrawHint1, hint1, FuncDrawHint2, hint2, FuncDrawHint3, hint3)                                                                                                                       \
static const StructHelpDrawButton hints##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 }, { FuncDrawHint3, hint3 } };                                                          \
static const GraphButtonDef name = { Item::Type::GraphButton, 0, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                     \
    funcPress, funcDraw, hints##name, 3};

#define DEF_GRAPH_BUTTON_HINTS_5(name, title, hint, keeper, funcActive, funcPress, funcDraw,                                                                                                \
    FuncDrawHint1, hint1, FuncDrawHint2, hint2, FuncDrawHint3, hint3, FuncDrawHint4, hint4, FuncDrawHint5, hint5)                                                                           \
static const StructHelpDrawButton hints##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 },                                                                                      \
    { FuncDrawHint3, hint3 }, { FuncDrawHint4, hint4 }, { FuncDrawHint5, hint5 } };                                                                                                         \
static const GraphButtonDef name = { Item::Type::GraphButton, 0, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                     \
    funcPress, funcDraw, hints##name, 5};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_CHOICE_2(name, title, hint, name1, name2, cell, keeper, funcActive, funcChanged, funcDraw)                                                                                      \
static const pString hints##name[] = {name1, name2};                                                                                                                                        \
static const ChoiceDef name = {Item::Type::Choice, 2, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                                \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_3(name, title, hint, name1, name2, name3, cell, keeper, funcActive, funcChanged, funcDraw)                                                                               \
static const pString hints##name[] = {name1, name2, name3};                                                                                                                                 \
static const ChoiceDef name = {Item::Type::Choice, 3, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                                \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_4(name, title, hint, name1, name2, name3, name4, cell, keeper, funcActive, funcChanged, funcDraw)                                                                        \
static const pString hints##name[] = {name1, name2, name3, name4};                                                                                                                          \
static const ChoiceDef name = {Item::Type::Choice, 4, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                                \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_5(name, title, hint, name1, name2, name3, name4, name5, cell, keeper, funcActive, funcChanged, funcDraw)                                                                 \
static pString hints##name[] = {name1, name2, name3, name4, name5};                                                                                                                         \
static const ChoiceDef name = {Item::Type::Choice, 5, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                                \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_6(name, title, hint, name1, name2, name3, name4, name5, name6, cell, keeper, funcActive, funcChanged, funcDraw)                                                          \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6};                                                                                                                  \
static const ChoiceDef name = {Item::Type::Choice, 6, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                                \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_7(name, title, hint, name1, name2, name3, name4, name5, name6, name7, cell, keeper, funcActive, funcChanged, funcDraw)                                                   \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6, name7};                                                                                                           \
static const ChoiceDef name = {Item::Type::Choice, 7, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                                \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_8(name, title, hint, name1, name2, name3, name4, name5,  name6, name7, name8, cell, keeper, funcActive, funcChanged, funcDraw)                                           \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6, name7, name8};                                                                                                    \
static const ChoiceDef name = {Item::Type::Choice, 8, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                         \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_9(name, title, hint, name1, name2, name3, name4, name5, name6, name7, name8, name9, cell, keeper, funcActive, funcChanged, funcDraw)                                     \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6, name7, name8, name9};                                                                                             \
static const ChoiceDef name = {Item::Type::Choice, 9, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                                \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_10(name, title, hint, name1, name2, name3, name4, name5, name6, name7, name8, name9, name10,                                                                             \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                                                        \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6, name7, name8, name9, name10};                                                                                     \
static const ChoiceDef name = {Item::Type::Choice, 10, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                               \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};


#define DEF_CHOICE_16(name, title, hint, name1, name2,  name3,  name4,  name5,  name6,  name7,  name8, name9, name10,                                                                       \
    name11, name12, name13, name14, name15, name16, cell, keeper, funcActive, funcChanged, funcDraw)                                                                                        \
static pString hints##name[] = {name1,  name2, name3,  name4,  name5,  name6, name7, name8, name9, name10, name11, name12, name13, name14, name15, name16 };                                \
static const ChoiceDef name = {Item::Type::Choice, 16, Page::Name::NoPage, keeper, funcActive, {title, hint},                                                                               \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_REG_9(name, titleRU, titleEN, hintRU, hintEN,                                                                                                                            \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5,                                                                                               \
    nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, cell, keeper, funcActive, funcChanged, funcDraw)                                                                \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,                                                                                     \
    nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9};                                                                                              \
static const ChoiceDef name = {Item::Type::ChoiceReg, 9, false, Page::Name::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                                \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_REG_10(name, titleRU, titleEN, hintRU, hintEN,                                                                                                                           \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5,                                                                                               \
    nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10, cell, keeper, funcActive, funcChanged, funcDraw)                                            \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,                                                                                     \
    nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10};                                                                          \
static const ChoiceDef name = {Item::Type::ChoiceReg, 10, false, Page::Name::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                                               \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};
