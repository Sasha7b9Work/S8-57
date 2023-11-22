// 2023/11/22 13:43:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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
    S_CURS_SHOW, &PageCursorsMeasures::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

static bool IsActive_ShowFreq()
{
    return S_CURS_SHOW;
}

DEF_CHOICE_2( cShowFreq,                                                                                                                                             //--- КУРОСРЫ - 1/dT ---
    "1/dT"
    ,
    "Если выбрано \"Вкл\", в правом верхнем углу выводится величина, обратная расстоянию между курсорами времени - частота сигнала, один период "
    "которого равен расстоянию между временными курсорами."
    ,
    DISABLE_RU,
    ENABLE_RU,
    S_CURS_SHOW_FREQ, &PageCursorsMeasures::self, IsActive_ShowFreq, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_3( pCursors,                                                                                                                                                     //--- ИЗМЕРЕНИЯ --- //-V2567
    "КУРСОРЫ",
    "Курсорные измерения",
    &cShow,
    &cShowFreq,
    PageCursorsMeasures::Set::self,
    PageName::CursorsMeasures, &PageMeasures::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageCursorsMeasures::self = static_cast<const Page *>(&pCursors);
