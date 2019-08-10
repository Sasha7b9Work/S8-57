#pragma once
#include "Utils/CommonFunctions.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_BUTTON(name, title, hint, keeper, funcActive, funcPress, funcDraw)                                                                          \
volatile static const ButtonBase name = { Control::Type::Button, 0, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                      \
    funcPress, funcDraw };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_TIME(name, title, hint, keeper, funcActive, cur, h, mi, s, mo, d, y)                                                                        \
static const TimeBase name = { Control::Type::Time, 0, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                                   \
    &cur, &h, &mi, &s, &mo, &d, &y };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GOVERNOR(name, title, hint, cell, min, max, keeper, funcActive, funcChanged, funcBeforeDraw)                                                \
volatile static const GovernorBase name = {Control::Type::Governor, 0, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                   \
    &(cell), min, max, funcChanged, funcBeforeDraw};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_IP_ADDRESS(name, titleRU, titleEN, hintRU, hintEN, ip0, ip1, ip2, ip3, port, keeper, funcActive, funcOfChanged)                             \
static const IPaddressBase name = {Item_IP, 0, false, Page::Name::NoPage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                      \
    &ip0, &ip1, &ip2, &ip3, funcOfChanged, port};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_MAC_ADDRESS(name, titleRU, titleEN, hintRU, hintEN, mac0, mac1, mac2, mac3, mac4, mac5, keeper, funcActive, funcOfChanged)                  \
static const MACaddressBase name = {Item_MAC, 0, false, Page::Name::NoPage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                    \
    &mac0, &mac1, &mac2, &mac3, &mac4, &mac5, funcOfChanged};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GOVERNOR_COLOR(name, title, hint, colorType, keeper)                                                                                        \
static const GovernorColorBase name = {Control::Type::GovernorColor, 0, false, Page::Name::NoPage, keeper, 0,                                          \
    {title, hint}, &colorType, 0};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_PAGE_SB(name, title, hint, item0, item1, item2, item3, item4, item5, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)         \
static const Control * const  items##name[] = {(Control *)item0, (Control *)item1, (Control *)item2, (Control *)item3, (Control *)item4,                \
    (Control *)item5};                                                                                                                                  \
static const PageBase name = {Control::Type::Page, 6, true, namePage, keeper, funcActive, {title, hint}, items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_1(name, title, hint, item1, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                             \
static const Control * const  items##name[] = {(Control *)item1};                                                                                       \
static const PageBase name = {Control::Type::Page, 1, false, namePage, keeper, funcActive, {title, hint},                                               \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_2(name, title, hint, item1, item2, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                      \
static const Control * const  items##name[] = {(Control *)item1, (Control *)item2};                                                                     \
static const PageBase name = {Control::Type::Page, 2, false, namePage, keeper, funcActive, {title, hint},                                               \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_3(name, title, hint, item1, item2, item3, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                               \
static const Control * items##name[] = {(Control *)(item1), (Control *)(item2), (Control *)(item3)};                                                    \
static const PageBase name = {Control::Type::Page, 3, false, namePage, keeper, funcActive, {title, hint},                                               \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_4(name, title, hint, item1, item2, item3, item4, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                        \
static const Control * items##name[] = {(Control *)item1, (Control *)item2, (Control *)item3, (Control *)item4};                                        \
static const PageBase name = {Control::Type::Page, 4, false, namePage, keeper, funcActive, {title, hint},                                               \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_5(name, title, hint,                                                                                                                   \
    item1, item2, item3, item4, item5, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                   \
static const Control * items##name[] = {(Control *)item1, (Control *)item2, (Control *)item3, (Control *)item4, (Control *)item5};                      \
static const PageBase name = {Control::Type::Page, 5, false, namePage, keeper, funcActive, {title, hint},                                               \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_6(name, title, hint, item1, item2, item3, item4, item5, item6, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)              \
static const Control * const  items##name[] = {(Control *)item1, (Control *)item2, (Control *)item3, (Control *)item4, (Control *)item5, (Control *)item6}; \
static const PageBase name = {Control::Type::Page, 6, false, namePage, keeper, funcActive, {title, hint},                                                   \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_7(name, title, hint,                                                                                                                   \
    item1, item2, item3, item4, item5, item6, item7, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                     \
static const Control * const  items##name[] = {                                                                                                         \
    (Control *)item1, (Control *)item2, (Control *)item3, (Control *)item4, (Control *)item5, (Control *)item6, (Control *)item7};                      \
static const PageBase name = {Control::Type::Page, 7, false, namePage, keeper, funcActive, {title, hint},                                               \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_8(name, title, hint, item1, item2, item3, item4, item5, item6, item7, item8, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)    \
static const Control * const  items##name[] = {(Control *)item1, (Control *)item2, (Control *)item3, (Control *)item4, (Control *)item5,                        \
    (Control *)item6, (Control *)item7, (Control *)item8};                                                                                                      \
static const PageBase name = {Control::Type::Page, 8, false, namePage, keeper, funcActive, {title, hint},                                                       \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_9(name, title, hint,                                                                                                                   \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                       \
static const Control * const  items##name[] = {                                                                                                         \
    (Control *)item1, (Control *)item2, (Control *)item3, (Control *)item4, (Control *)item5, (Control *)item6, (Control *)item7,                       \
    (Control *)item8, (Control *)item9};                                                                                                                \
static const PageBase name = {Control::Type::Page, 9, false, namePage, keeper, funcActive, {title, hint},                                               \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_10(name, titleRU, titleEN, hintRU, hintEN,                                                                                             \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)               \
static const Control * const  items##name[] = {                                                                                                         \
    (Control *)item1, (Control *)item2, (Control *)item3, (Control *)item4, (Control *)item5, (Control *)item6, (Control *)item7,                       \
    (Control *)item8, (Control *)item9, (Control *)item10};                                                                                             \
static const PageBase name = {Control::Type::Page, 10, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                         \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_11(name, titleRU, titleEN, hintRU, hintEN,                                                                                             \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)       \
static const Control * const  items##name[] = {(Control *)item1, (Control *)item2, (Control *)item3, (Control *)item4, (Control *)item5,                \
    (Control *)item6, (Control *)item7, (Control *)item8, (Control *)item9, (Control *)item10, (Control *)item11};                                      \
static const PageBase name = {Control::Type::Page, 11, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                         \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_12(name, title, hint,                                                                                              \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet) \
static const Control * const  items##name[] = {(Control *)item1, (Control *)item2, (Control *)item3, (Control *)item4, (Control *)item5,                \
    (Control *)item6, (Control *)item7, (Control *)item8, (Control *)item9, (Control *)item10, (Control *)item11, (Control *)item12};                   \
static const PageBase name = {Control::Type::Page, 12, false, namePage, keeper, funcActive, {title, hint},                                              \
    items##name, funcPress, funcDraw, funcRegSet};

#define DEF_PAGE_13(name, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                                    \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, item13)                                                      \
static const Control * const  items##name[] = {                                                                                                         \
    (Control *)item1, (Control *)item2, (Control *)item3, (Control *)item4, (Control *)item5, (Control *)item6, (Control *)item7,                       \
    (Control *)item8, (Control *)item9, (Control *)item10, (Control *)item11, (Control *)item12, (Control *)item13};                                    \
static const PageBase name = {Control::Type::Page, 13, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                         \
    items##name, funcPress, FuncDrawPage, 0};

#define DEF_PAGE_15(name, namePage, keeper, funcActive, funcPress, titleRU, titleEN, hintRU, hintEN,                                                    \
    item1, item2, item3, item4, item5, item6, item7, item8, item9, item10,                                                                              \
    item11, item12, item13, item14, item15)                                                                                                             \
static const Control * const  items##name[] = {(Control *)item1, (Control *)item2, (Control *)item3, (Control *)item4, (Control *)item5,                \
    (Control *)item6, (Control *)item7, (Control *)item8, (Control *)item9, (Control *)item10, (Control *)item11, (Control *)item12,                    \
    (Control *)item13, (Control *)item14, (Control *)item15};                                                                                           \
static const PageBase name = {Control::Type::Page, 15, false, namePage, keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},                         \
    items##name, funcPress, FuncDrawPage, 0};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_CHOICE_PARAMETER(name, titleRU, titleEN, hintRU, hintEN, keeper, funcActive, funcPress, numParam,                                           \
    allow0, allow1, allow2, allow3, allow4, allow5, allow6, allow7)                                                                                     \
ChoiceParameterBase name = {Item_ChoiceParameter, 0, false, Page::Name::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN} ,               \
    funcPress, {allow0, allow1, allow2, allow3, allow4, allow5, allow6, allow7}, (uint8 *)&numParam};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_SMALL_BUTTON(name, title, hint, keeper, funcActive, funcPress, funcDraw)                                                                    \
volatile static const SButtonBase name = { Control::Type::DrawButton, 0, false, Page::Name::NoPage, keeper, funcActive,                                \
     {title, hint}, funcPress, funcDraw, 0, 0};

#define DEF_SMALL_BUTTON_EXIT(name, keeper, funcActive, funcPress, funcDraw)                                                                            \
static const SButtonBase name = { Control::Type::DrawButton, 0, false, Page::Name::NoPage, keeper, funcActive, {"Выход",                               \
    "Кнопка для выхода в предыдущее меню"}, funcPress, funcDraw, 0, 0};

#define DEF_SMALL_BUTTON_HINTS_2(name, title, hint, keeper, funcActive, funcPress, funcDraw, FuncDrawHint1, hint1, FuncDrawHint2, hint2)                \
static const StructHelpDrawButton hints##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 }};                                                 \
volatile static const SButtonBase name = { Control::Type::DrawButton, 0, false, Page::Name::NoPage, keeper, funcActive,                                \
     {title, hint}, funcPress, funcDraw, hints##name, 2};

#define DEF_SMALL_BUTTON_HINTS_3(name, title, hint, keeper, funcActive, funcPress, funcDraw,                                                            \
    FuncDrawHint1, hint1, FuncDrawHint2, hint2, FuncDrawHint3, hint3)                                                                                   \
static const StructHelpDrawButton hints##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 }, { FuncDrawHint3, hint3 } };                      \
static const SButtonBase name = { Control::Type::DrawButton, 0, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                          \
    funcPress, funcDraw, hints##name, 3};

#define DEF_SMALL_BUTTON_HINTS_5(name, title, hint, keeper, funcActive, funcPress, funcDraw,                                                            \
    FuncDrawHint1, hint1, FuncDrawHint2, hint2, FuncDrawHint3, hint3, FuncDrawHint4, hint4, FuncDrawHint5, hint5)                                       \
static const StructHelpDrawButton hints##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 },                                                  \
    { FuncDrawHint3, hint3 }, { FuncDrawHint4, hint4 }, { FuncDrawHint5, hint5 } };                                                                     \
static const SButtonBase name = { Control::Type::DrawButton, 0, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                          \
    funcPress, funcDraw, hints##name, 5};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_CHOICE_2(name, title, hint, name1, name2, cell, keeper, funcActive, funcChanged, funcDraw)                                                  \
static const pString hints##name[] = {name1, name2};                                                                                                    \
volatile static const ChoiceBase name = {Control::Type::Choice, 2, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                       \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_3(name, title, hint, name1, name2, name3, cell, keeper, funcActive, funcChanged, funcDraw)                                           \
static const pString hints##name[] = {name1, name2, name3};                                                                                             \
volatile static const ChoiceBase name = {Control::Type::Choice, 3, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                       \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_4(name, title, hint, name1, name2, name3, name4, cell, keeper, funcActive, funcChanged, funcDraw)                                    \
static const pString hints##name[] = {name1, name2, name3, name4};                                                                                      \
volatile static const ChoiceBase name = {Control::Type::Choice, 4, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                       \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_5(name, title, hint, name1, name2, name3, name4, name5, cell, keeper, funcActive, funcChanged, funcDraw)                             \
static pString hints##name[] = {name1, name2, name3, name4, name5};                                                                                     \
volatile static const ChoiceBase name = {Control::Type::Choice, 5, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                       \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_6(name, title, hint, name1, name2, name3, name4, name5, name6, cell, keeper, funcActive, funcChanged, funcDraw)                      \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6};                                                                              \
static const ChoiceBase name = {Control::Type::Choice, 6, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                                \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_7(name, title, hint, name1, name2, name3, name4, name5, name6, name7, cell, keeper, funcActive, funcChanged, funcDraw)               \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6, name7};                                                                       \
static const ChoiceBase name = {Control::Type::Choice, 7, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                                \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_8(name, title, hint, name1, name2, name3, name4, name5,  name6, name7, name8, cell, keeper, funcActive, funcChanged, funcDraw)       \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6, name7, name8};                                                                \
static const ChoiceBase name = {Control::Type::Choice, 8, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                                \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_9(name, title, hint, name1, name2, name3, name4, name5, name6, name7, name8, name9, cell, keeper, funcActive, funcChanged, funcDraw) \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6, name7, name8, name9};                                                         \
static const ChoiceBase name = {Control::Type::Choice, 9, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                                \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_10(name, title, hint, name1, name2, name3, name4, name5, name6, name7, name8, name9, name10,                                         \
    cell, keeper, funcActive, funcChanged, funcDraw)                                                                                                    \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6, name7, name8, name9, name10};                                                 \
static const ChoiceBase name = {Control::Type::Choice, 10, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                               \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};


#define DEF_CHOICE_16(name, title, hint, name1, name2,  name3,  name4,  name5,  name6,  name7,  name8, name9, name10,                                           \
    name11, name12, name13, name14, name15, name16, cell, keeper, funcActive, funcChanged, funcDraw)                                                            \
static pString hints##name[] = {name1,  name2, name3,  name4,  name5,  name6, name7, name8, name9, name10, name11, name12, name13, name14, name15, name16 };    \
volatile static const ChoiceBase name = {Control::Type::Choice, 16, false, Page::Name::NoPage, keeper, funcActive, {title, hint},                              \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_REG_9(name, titleRU, titleEN, hintRU, hintEN,                                                                                        \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5,                                                           \
    nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, cell, keeper, funcActive, funcChanged, funcDraw)                            \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,                                                 \
    nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9};                                                          \
static const ChoiceBase name = {Control::Type::ChoiceReg, 9, false, Page::Name::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},        \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_REG_10(name, titleRU, titleEN, hintRU, hintEN,                                                                                       \
    nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4, nameRu5, nameEn5,                                                           \
    nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10, cell, keeper, funcActive, funcChanged, funcDraw)        \
static pString hints##name[] = {nameRu1, nameEn1, nameRu2, nameEn2, nameRu3, nameEn3, nameRu4, nameEn4,                                                 \
    nameRu5, nameEn5, nameRu6, nameEn6, nameRu7, nameEn7, nameRu8, nameEn8, nameRu9, nameEn9, nameRu10, nameEn10};                                      \
static const ChoiceBase name = {Control::Type::ChoiceReg, 10, false, Page::Name::NoPage, &keeper, funcActive, {titleRU, titleEN, hintRU, hintEN},       \
    (int8 *)&cell, hints##name, funcChanged, funcDraw};
