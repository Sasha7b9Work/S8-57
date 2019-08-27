#pragma once
#include "defines.h"
#include "Utils/CommonFunctions.h"


typedef bool(*pFuncBKE)(KeyEvent);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_BUTTON(name, title, hint, keeper, funcActive, funcPress, funcDraw)                                                                                                              \
static const char *name##titleHint[] = {title, hint};                                                                                                                                       \
static const Button name(name##titleHint, keeper, funcActive, funcPress, funcDraw);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GOVERNOR(name, title, hint, cell, min, max, keeper, funcActive, funcChanged, funcBeforeDraw)                                                                                    \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Governor name(th##name, (int16 *)cell, min, max, keeper, funcActive, funcChanged, funcBeforeDraw);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GOVERNOR_COLOR(name, title, hint, colorType, keeper)                                                                                                                            \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const GovernorColor name(th##name, &colorType, keeper, 0, 0);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_PAGE_1(name, title, hint, item1, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                                 \
static const Item * const i##name[] = {(Item *)item1};                                                                                                                                      \
static const char * th##name[] = {title, hint};                                                                                                                                             \
static const Page name(th##name, keeper, i##name, 1, funcActive, funcPress, funcDraw, funcRegSet);

#define DEF_PAGE_2(name, title, hint, item1, item2, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                          \
static const Item * const  i##name[] = {(Item *)item1, (Item *)item2};                                                                                                                      \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Page name(th##name, keeper, i##name, 2, funcActive ,funcPress, funcDraw, funcRegSet);

#define DEF_PAGE_3(name, title, hint, item1, item2, item3, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                                   \
static const Item * const i##name[] = {(Item *)(item1), (Item *)(item2), (Item *)(item3)};                                                                                                  \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Page name(th##name, keeper, i##name, 3, funcActive, funcPress, funcDraw, funcRegSet);

#define DEF_PAGE_4(name, title, hint, item1, item2, item3, item4, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                            \
static const Item * const i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4};                                                                                         \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Page name(th##name, keeper, i##name, 4, funcActive, funcPress, funcDraw, funcRegSet);

#define DEF_PAGE_5(name, title, hint,  item1, item2, item3, item4, item5, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                    \
static const Item * const i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5};                                                                          \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Page name(th##name, keeper, i##name, 5, funcActive ,funcPress, funcDraw, funcRegSet);

#define DEF_PAGE_5_VAR(name, title, hint, item1, item2, item3, item4, item5, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                                 \
static const Item *i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5};                                                                                 \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Page name(th##name, keeper, i##name, 5, funcActive ,funcPress, funcDraw, funcRegSet);

#define DEF_PAGE_6(name, title, hint, item1, item2, item3, item4, item5, item6, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                              \
static const Item * const i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6};                                                           \
static const char * const th##name[] = {title, hint};                                                                                                                                       \
static const Page name(th##name, keeper, i##name, 6, funcActive, funcPress, funcDraw, funcRegSet);

#define DEF_PAGE_7(name, title, hint, item1, item2, item3, item4, item5, item6, item7, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                       \
static const Item * const  i##name[] = { (Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6, (Item *)item7};                                          \
static const char * const th##name[] = {title, hint};                                                                                                                                       \
static const Page name(th##name, keeper, i##name, 7, funcActive, funcPress, funcDraw, funcRegSet);

#define DEF_PAGE_8(name, title, hint, item1, item2, item3, item4, item5, item6, item7, item8, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)                                \
static const Item * const  i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5, (Item *)item6, (Item *)item7, (Item *)item8};                            \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Page name(th##name, keeper, &i##name[0], 8, funcActive, funcPress, funcDraw, funcRegSet);

#define DEF_PAGE_12(name, title, hint, item1, item2, item3, item4, item5, item6, item7, item8, item9, item10, item11, item12, namePage, keeper, funcActive, funcPress, funcDraw, funcRegSet)\
static const Item * const  i##name[] = {(Item *)item1, (Item *)item2, (Item *)item3, (Item *)item4, (Item *)item5,                                                                          \
    (Item *)item6, (Item *)item7, (Item *)item8, (Item *)item9, (Item *)item10, (Item *)item11, (Item *)item12};                                                                            \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Page name(th##name, keeper, i##name, 12, funcActive, funcPress, funcDraw, funcRegSet);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_GRAPH_BUTTON(name, title, hint, keeper, funcActive, funcPress, funcDraw)                                                                                                        \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const GraphButton name(th##name, nullptr, 0, keeper, funcActive, funcPress, funcDraw);

#define DEF_GRAPH_BUTTON_HINTS_2(name, title, hint, keeper, funcActive, funcPress, funcDraw, FuncDrawHint1, hint1, FuncDrawHint2, hint2)                                                    \
static const StructHelpDrawButton h##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 }};                                                                                         \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const GraphButton name(th##name, h##name, 2, keeper, funcActive, funcPress, funcDraw);

#define DEF_GRAPH_BUTTON_HINTS_3(name, title, hint, keeper, funcActive, funcPress, funcDraw, FuncDrawHint1, hint1, FuncDrawHint2, hint2, FuncDrawHint3, hint3)                              \
static const StructHelpDrawButton h##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 }, { FuncDrawHint3, hint3 } };                                                              \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const GraphButton name(th##name, h##name, 3, keeper, funcActive, funcPress, funcDraw);

#define DEF_GRAPH_BUTTON_HINTS_5(name, title, hint, keeper, funcActive, funcPress, funcDraw,                                                                                                \
    FuncDrawHint1, hint1, FuncDrawHint2, hint2, FuncDrawHint3, hint3, FuncDrawHint4, hint4, FuncDrawHint5, hint5)                                                                           \
static const StructHelpDrawButton h##name[] = {{ FuncDrawHint1, hint1 }, { FuncDrawHint2, hint2 },                                                                                          \
    { FuncDrawHint3, hint3 }, { FuncDrawHint4, hint4 }, { FuncDrawHint5, hint5 } };                                                                                                         \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const GraphButton name(th##name, h##name, 5, keeper, funcActive, funcPress, funcDraw);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEF_CHOICE_2(name, title, hint, name1, name2, cell, keeper, funcActive, funcChanged, funcDraw)                                                                                      \
static const pString name##names[] = {name1, name2};                                                                                                                                        \
static const char * const th##name[] = {title, hint};                                                                                                                                       \
static const Choice name(&th##name[0], name##names, 2, (int8 *)&cell, keeper, funcActive, funcChanged, funcDraw);

#define DEF_CHOICE_3(name, title, hint, name1, name2, name3, cell, keeper, funcActive, funcChanged, funcDraw)                                                                               \
static const pString n##name[] = {name1, name2, name3};                                                                                                                                     \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Choice name(th##name, n##name, 3, (int8 *)&cell, keeper, funcActive, funcChanged, funcDraw);

#define DEF_CHOICE_4(name, title, hint, name1, name2, name3, name4, cell, keeper, funcActive, funcChanged, funcDraw)                                                                        \
static const pString n##name[] = {name1, name2, name3, name4};                                                                                                                              \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Choice name(th##name, n##name, 4, (int8 *)&cell, keeper, funcActive ,funcChanged, funcDraw);

#define DEF_CHOICE_5(name, title, hint, name1, name2, name3, name4, name5, cell, keeper, funcActive, funcChanged, funcDraw)                                                                 \
static const pString n##name[] = {name1, name2, name3, name4, name5};                                                                                                                       \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Choice name(th##name, n##name, 5, (int8 *)&cell, keeper, funcActive, funcChanged, funcDraw);

#define DEF_CHOICE_6(name, title, hint, name1, name2, name3, name4, name5, name6, cell, keeper, funcActive, funcChanged, funcDraw)                                                          \
static pString n##name[] = {name1, name2, name3, name4, name5, name6};                                                                                                                      \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Choice name(th##name, n##name, 6, (int8 *)&cell, keeper, funcActive, funcChanged, funcDraw);

#define DEF_CHOICE_7(name, title, hint, name1, name2, name3, name4, name5, name6, name7, cell, keeper, funcActive, funcChanged, funcDraw)                                                   \
static pString n##name[] = {name1, name2, name3, name4, name5, name6, name7};                                                                                                               \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Choice name(th##name, n##name, 7, (int8 *)&cell, keeper, funcActive, funcChanged, funcDraw);

#define DEF_CHOICE_8(name, title, hint, name1, name2, name3, name4, name5,  name6, name7, name8, cell, keeper, funcActive, funcChanged, funcDraw)                                           \
static pString hints##name[] = {name1, name2, name3, name4, name5, name6, name7, name8};                                                                                                    \
static const ChoiceDef name = {Item::Type::Choice, 8, false, Page::Name::NoPage, keeper, funcActive, {title, hint},(int8 *)&cell, hints##name, funcChanged, funcDraw};

#define DEF_CHOICE_9(name, title, hint, name1, name2, name3, name4, name5, name6, name7, name8, name9, cell, keeper, funcActive, funcChanged, funcDraw)                                     \
static pString n##name[] = {name1, name2, name3, name4, name5, name6, name7, name8, name9};                                                                                                 \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Choice name(th##name, n##name, 9, (int8 *)&cell, keeper, funcActive, funcChanged, funcDraw);

#define DEF_CHOICE_10(name, title, hint, name1, name2, name3, name4, name5, name6, name7, name8, name9, name10, cell, keeper, funcActive, funcChanged, funcDraw)                            \
static pString n##name[] = {name1, name2, name3, name4, name5, name6, name7, name8, name9, name10};                                                                                         \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Choice name(th##name, n##name, 10, (int8 *)&cell, keeper, funcActive, funcChanged, funcDraw);


#define DEF_CHOICE_16(name, title, hint, name1, name2,  name3,  name4,  name5,  name6,  name7,  name8, name9, name10,                                                                       \
    name11, name12, name13, name14, name15, name16, cell, keeper, funcActive, funcChanged, funcDraw)                                                                                        \
static pString n##name[] = {name1,  name2, name3,  name4,  name5,  name6, name7, name8, name9, name10, name11, name12, name13, name14, name15, name16 };                                    \
static const char *th##name[] = {title, hint};                                                                                                                                              \
static const Choice name(th##name, n##name, 16, (int8 *)&cell, keeper, funcActive, funcChanged, funcDraw);
