#include "defines.h"
#include "Display/Painter.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Settings/Settings.h"
#include "Keyboard/Keyboard.h"
#include "Hardware/Beeper.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Utils/CommonFunctions.h"
#include <cmath>
#include <cstring>



DEF_CHOICE_2( cShow,                                                                                                                                           //--- КУРСОРЫ - Показывать ---
    "Показывать",
    "Включает/отключает курсоры.",
    "Нет",
    "Да",
    set.curs.showCursors, &PageCursorsMeasures::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

static bool IsActive_ShowFreq()
{
    return set.curs.showCursors;
}

DEF_CHOICE_2( cShowFreq,                                                                                                                                             //--- КУРОСРЫ - 1/dT ---
    "1/dT"
    ,
    "Если выбрано \"Вкл\", в правом верхнем углу выводится величина, обратная расстоянию между курсорами времени - частота сигнала, один период "
    "которого равен расстоянию между временными курсорами."
    ,
    DISABLE_RU,
    ENABLE_RU,
    set.curs.showFreq, &PageCursorsMeasures::self, IsActive_ShowFreq, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_3( pCursors,                                                                                                                                    //--- ИЗМЕРЕНИЯ ---
    "КУРСОРЫ",
    "Курсорные измерения",
    &cShow,
    &cShowFreq,
    PageCursorsMeasures::PageSet::self,
    PageName::CursorsMeasures, &PageMeasures::self, Item::Active, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageCursorsMeasures::self = (const Page *)&pCursors;
