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


//_DEF_CHOICE_4( cLookModeChanA,                                                                                                                            //--- КУРСОРЫ - Слежение канал 1 ---
//    "Слежение \x8e, \x9e"
//    ,
//    "Задаёт режим слежения за первым курсором времени и напряжения:\n"
//    "1. \"Откл\" - курсор времени и курсор напряжения устанавливаются вручную.\n"
//    "2. \"Напряжение\" - при ручном изменении положения курсора времени курсор напряжения автоматически отслеживают изменения сигнала.\n"
//    "3. \"Время\" - при ручном изменении положения курсора напряжения курсор времени автоматически отслеживают изменения сигнала.\n"
//    "4. \"Напряж и время\" - действует как один из предыдущих режимов, в зависимости от того, на какой курсор производилось последнее воздействие."
//    ,
//    DISABLE_RU,
//    "Напряжение",
//    "Время",
//    "Напряж и время",
//    CURS_LOOK_MODE(Chan::A), &PageCursorsMeasures::self, 0, 0, 0
//)


//_DEF_CHOICE_4( cLookModeChanB,                                                                                                                            //--- КУРСОРЫ - Слежение канал 2 ---
//    "Слежение \x8f, \x9f"
//    ,
//    "Задаёт режим слежения за вторым курсором времени и напряжения:\n"
//    "1. \"Откл\" - курсор времени и курсор напряжения устанавливаются вручную.\n"
//    "2. \"Напряжение\" - при ручном изменении положения курсора времени курсор напряжения автоматически отслеживают изменения сигнала.\n"
//    "3. \"Время\" - при ручном изменении положения курсора напряжения курсор времени автоматически отслеживают изменения сигнала.\n"
//    "4. \"Напряж и время\" - действует как один из предыдущих режимов, в зависимости от того, на какой курсор производилось последнее воздействие."
//    ,
//    DISABLE_RU,
//    "Напряжение",
//    "Время",
//    "Напряж. и время",
//    CURS_LOOK_MODE(Chan::B), &PageCursorsMeasures::self, 0, 0, 0
//)


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


DEF_PAGE_3( pCursors, // -V641 // -V1027                                                                                                                                  //--- ИЗМЕРЕНИЯ ---
    "КУРСОРЫ",
    "Курсорные измерения",
    &cShow,
//    &cLookModeChanA,
//    &cLookModeChanB,
    &cShowFreq,
    PageCursorsMeasures::PageSet::self,
    PageName::CursorsMeasures, &PageMeasures::self, Item::Active, Page::Changed, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageCursorsMeasures::self = (const Page *)&pCursors;
