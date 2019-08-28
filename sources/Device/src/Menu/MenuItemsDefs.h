#pragma once
#include "defines.h"
#include "Utils/CommonFunctions.h"


typedef bool(*pFuncBKE)(KeyEvent);

inline bool EmptyFuncfBKE(KeyEvent) { return false; }


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_BUTTON(name, title, hint, keeper, funcActive, funcPress, funcDraw)                                                                                                              \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::Button, 0, Page::Name::NoPage, keeper, funcActive, th##name };                                                                               \
static const Button name(&hi##name, funcPress, funcDraw);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GOVERNOR(name, title, hint, cell, min, max, keeper, funcActive, funcChanged, funcBeforeDraw)                                                                                    \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::Button, 0, Page::Name::NoPage, keeper, funcActive, th##name };                                                                               \
static const Governor name(&hi##name, (int16 *)&cell, min, max, funcChanged, funcBeforeDraw);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GOVERNOR_COLOR(name, title, hint, colorType, keeper)                                                                                                                            \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::GovernorColor, 0, Page::Name::NoPage, keeper, EmptyFuncBtV, th##name };                                                                      \
static const GovernorColor name(&hi##name, &colorType, 0);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GRAPH_BUTTON(name, title, hint, keeper, funcActive, funcPress, funcDraw)                                                                                                        \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::GovernorColor, 0, Page::Name::NoPage, keeper, funcActive, th##name };                                                                        \
static const GraphButton name(&hi##name, nullptr, funcPress, funcDraw);

#define DEF_GRAPH_BUTTON_HINTS_2(name, title, hint, keeper, funcActive, funcPress, funcDraw, FuncDrawHint1, hint1, FuncDrawHint2, hint2)                                                    \
static const StructHelpDrawButton h##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 }};                                                                                         \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::GovernorColor, 2, Page::Name::NoPage, keeper, funcActive, th##name };                                                                        \
static const GraphButton name(&hi##name, h##name, funcPress, funcDraw);

#define DEF_GRAPH_BUTTON_HINTS_3(name, title, hint, keeper, funcActive, funcPress, funcDraw, FuncDrawHint1, hint1, FuncDrawHint2, hint2, FuncDrawHint3, hint3)                              \
static const StructHelpDrawButton h##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 }, { FuncDrawHint3, hint3 } };                                                              \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::GovernorColor, 3, Page::Name::NoPage, keeper, funcActive, th##name };                                                                        \
static const GraphButton name(&hi##name, h##name, funcPress, funcDraw);

#define DEF_GRAPH_BUTTON_HINTS_5(name, title, hint, keeper, funcActive, funcPress, funcDraw,                                                                                                \
    FuncDrawHint1, hint1, FuncDrawHint2, hint2, FuncDrawHint3, hint3, FuncDrawHint4, hint4, FuncDrawHint5, hint5)                                                                           \
static const StructHelpDrawButton h##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 },                                                                                          \
    { FuncDrawHint3, hint3 }, { FuncDrawHint4, hint4 }, { FuncDrawHint5, hint5 } };                                                                                                         \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::GovernorColor, 5, Page::Name::NoPage, keeper, funcActive, th##name };                                                                        \
static const GraphButton name(&hi##name, h##name, funcPress, funcDraw);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_PAGE_1(name, title, hint, item1, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                                 \
static const Item * const i##name[] = {(Item *)item1};                                                                                                                                      \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataPage dp##name = {i##name, funcPress, funcDraw, funcRegSet};                                                                                                                \
static const DataItem di##name = { Item::Type::Page, 1, namePage, keeper, funcActive, th##name, &dp##name };                                                                                \
static const Page name(&di##name);

#define DEF_PAGE_2(name, title, hint, item1, item2, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                          \
static const Item * const  i##name[] = {(Item *)item1, (Item *)item2};                                                                                                                      \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataPage dp##name = {i##name, funcPress, funcDraw, funcRegSet};                                                                                                                \
static const DataItem di##name = { Item::Type::Page, 2, namePage, keeper, funcActive, th##name, &dp##name };                                                                                \
static const Page name(&di##name);

#define DEF_PAGE_3(name, title, hint, item1, item2, item3, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                   \
static const Item * const i##name[] = {(Item *)(item1), (Item *)(item2), (Item *)(item3)};                                                                                                  \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataPage dp##name = {i##name, funcPress, funcDraw, funcRegSet};                                                                                                                \
static const DataItem di##name = { Item::Type::Page, 3, namePage, keeper, funcActive, th##name, &dp##name};                                                                                 \
static const Page name(&di##name);

#define DEF_PAGE_4(name, title, hint, item1, item2, item3, item4, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                            \
static const Item * const i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4};                                                                                         \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataPage dp##name = {i##name, funcPress, funcDraw, funcRegSet};                                                                                                                \
static const DataItem di##name = { Item::Type::Page, 4, namePage, keeper, funcActive, th##name, &dp##name };                                                                                \
static const Page name(&di##name);

#define DEF_PAGE_5(name, title, hint,  item1, item2, item3, item4, item5, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                    \
static const Item * const i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5};                                                                          \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataPage dp##name = {i##name, funcPress, funcDraw, funcRegSet};                                                                                                                \
static const DataItem di##name = { Item::Type::Page, 5, namePage, keeper, funcActive, th##name, &dp##name };                                                                                \
static const Page name(&di##name);

#define DEF_PAGE_5_VAR(name, title, hint,  item1, item2, item3, item4, item5, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                \
static const Item *i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5};                                                                                 \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataPage dp##name = {i##name, funcPress, funcDraw, funcRegSet};                                                                                                                \
static const DataItem di##name = { Item::Type::Page, 5, namePage, keeper, funcActive, th##name, &dp##name };                                                                                \
static const Page name(&di##name);

#define DEF_PAGE_6(name, title, hint, item1, item2, item3, item4, item5, item6, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                              \
static const Item * const i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6};                                                           \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataPage dp##name = {i##name, funcPress, funcDraw, funcRegSet};                                                                                                                \
static const DataItem di##name = { Item::Type::Page, 6, namePage, keeper, funcActive, th##name, &dp##name };                                                                                \
static const Page name(&di##name);

#define DEF_PAGE_7(name, title, hint, item1, item2, item3, item4, item5, item6, item7, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                       \
static const Item * const  i##name[] = { (Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6, (Item *)item7};                                          \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataPage dp##name = {i##name, funcPress, funcDraw, funcRegSet};                                                                                                                \
static const DataItem di##name = { Item::Type::Page, 7, namePage, keeper, funcActive, th##name, &dp##name };                                                                                \
static const Page name(&di##name);

#define DEF_PAGE_8(name, title, hint, item1, item2, item3, item4, item5, item6, item7, item8, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                \
static const Item * const  i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6, (Item *)item7, (Item *)item8};                            \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataPage dp##name = {i##name, funcPress, funcDraw, funcRegSet};                                                                                                                \
static const DataItem di##name = { Item::Type::Page, 8, namePage, keeper, funcActive, th##name, &dp##name };                                                                                \
static const Page name(&di##name);

#define DEF_PAGE_12(name, title, hint, item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)\
static const Item * const  i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5,                                                                          \
    (Item *)item6, (Item *)item7, (Item *)item8, (Item *)item9, (Item *)item10, (Item *)item11, (Item *)item12};                                                                            \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataPage dp##name = {i##name, funcPress, funcDraw, funcRegSet};                                                                                                                \
static const DataItem di##name = { Item::Type::Page, 12, namePage, keeper, funcActive, th##name, &dp##name };                                                                               \
static const Page name(&di##name);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_CHOICE_2(name, title, hint, name1, name2, cell, keeper, funcActive, funcChanged, funcDraw)                                                                                      \
static const pString n##name[] = {name1, name2};                                                                                                                                            \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::Choice, 2, Page::Name::NoPage, keeper, funcActive, th##name };                                                                               \
static const Choice name(&hi##name, n##name, (int8 *)&cell, funcChanged, funcDraw);

#define DEF_CHOICE_3(name, title, hint, name1, name2, name3, cell, keeper, funcActive, funcChanged, funcDraw)                                                                               \
static const pString n##name[] = {name1, name2, name3};                                                                                                                                     \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::Choice, 3, Page::Name::NoPage, keeper, funcActive, th##name };                                                                               \
static const Choice name(&hi##name, n##name, (int8 *)&cell, funcChanged, funcDraw);

#define DEF_CHOICE_4(name, title, hint, name1, name2, name3, name4, cell, keeper, funcActive, funcChanged, funcDraw)                                                                        \
static const pString n##name[] = {name1, name2, name3, name4};                                                                                                                              \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::Choice, 4, Page::Name::NoPage, keeper, funcActive, th##name };                                                                               \
static const Choice name(&hi##name, n##name, (int8 *)&cell, funcChanged, funcDraw);

#define DEF_CHOICE_5(name, title, hint, name1, name2, name3, name4, name5, cell, keeper, funcActive, funcChanged, funcDraw)                                                                 \
static const pString n##name[] = {name1, name2, name3, name4, name5};                                                                                                                       \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::Choice, 5, Page::Name::NoPage, keeper, funcActive, th##name };                                                                               \
static const Choice name(&hi##name, n##name, (int8 *)&cell, funcChanged, funcDraw);

#define DEF_CHOICE_6(name, title, hint, name1, name2, name3, name4, name5, name6, cell, keeper, funcActive, funcChanged, funcDraw)                                                          \
static const pString n##name[] = {name1, name2, name3, name4, name5, name6};                                                                                                                \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::Choice, 6, Page::Name::NoPage, keeper, funcActive, th##name };                                                                               \
static const Choice name(&hi##name, n##name, (int8 *)&cell, funcChanged, funcDraw);

#define DEF_CHOICE_7(name, title, hint, name1, name2, name3, name4, name5, name6, name7, cell, keeper, funcActive, funcChanged, funcDraw)                                                   \
static const pString n##name[] = {name1, name2, name3, name4, name5, name6, name7};                                                                                                         \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::Choice, 7, Page::Name::NoPage, keeper, funcActive, th##name };                                                                               \
static const Choice name(&hi##name, n##name, (int8 *)&cell, funcChanged, funcDraw);

#define DEF_CHOICE_9(name, title, hint, name1, name2, name3, name4, name5, name6, name7, name8, name9, cell, keeper, funcActive, funcChanged, funcDraw)                                     \
static const pString n##name[] = {name1, name2, name3, name4, name5, name6, name7, name8, name9};                                                                                           \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::Choice, 9, Page::Name::NoPage, keeper, funcActive, th##name };                                                                               \
static const Choice name(&hi##name, n##name, (int8 *)&cell, funcChanged, funcDraw);

#define DEF_CHOICE_10(name, title, hint, name1, name2, name3, name4, name5, name6, name7, name8, name9, name10, cell, keeper, funcActive, funcChanged, funcDraw)                            \
static const pString n##name[] = {name1, name2, name3, name4, name5, name6, name7, name8, name9, name10};                                                                                   \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::Choice, 10, Page::Name::NoPage, keeper, funcActive, th##name };                                                                               \
static const Choice name(&hi##name, n##name, (int8 *)&cell, funcChanged, funcDraw);


#define DEF_CHOICE_16(name, title, hint, name1, name2,  name3,  name4,  name5,  name6,  name7,  name8, name9, name10,                                                                       \
    name11, name12, name13, name14, name15, name16, cell, keeper, funcActive, funcChanged, funcDraw)                                                                                        \
static const pString n##name[] = {name1,  name2, name3,  name4,  name5,  name6, name7, name8, name9, name10, name11, name12, name13, name14, name15, name16 };                              \
static const pString th##name[] = {title, hint};                                                                                                                                            \
static const DataItem hi##name = { Item::Type::Choice, 16, Page::Name::NoPage, keeper, funcActive, th##name };                                                                               \
static const Choice name(&hi##name, n##name, (int8 *)&cell, funcChanged, funcDraw);
