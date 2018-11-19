#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Display/Painter.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Settings/Settings.h"
#include "Keyboard/Keyboard.h"
#include "Hardware/Sound.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include <math.h>
#include <cstring>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageCursors;

const PageBase *PageMeasures::PageCursors::pointer = &pageCursors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2(       cShow,                                                                                              //--- КУРСОРЫ - Показывать ---
    "Показывать", "Shown",
    "Включает/отключает курсоры.",
    "Enable/disable cursors.",
    "Нет", "No",
    "Да",  "Yes",
    CURS_SHOW, pageCursors, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(       cLookModeChanA,                                                                               //--- КУРСОРЫ - Слежение канал 1 ---
    "Слежение \x8e, \x9e", "Tracking \x8e, \x9e"
    ,
    "Задаёт режим слежения за первым курсором времени и напряжения:\n"
    "1. \"Откл\" - курсор времени и курсор напряжения устанавливаются вручную.\n"
    "2. \"Напряжение\" - при ручном изменении положения курсора времени курсор напряжения автоматически отслеживают изменения сигнала.\n"
    "3. \"Время\" - при ручном изменении положения курсора напряжения курсор времени автоматически отслеживают изменения сигнала.\n"
    "4. \"Напряж и время\" - действует как один из предыдущих режимов, в зависимости от того, на какой курсор производилось последнее воздействие."
    ,
    /// \todo Перевод
    "Sets the mode tracking for second cursor:\n"
    "1. \"Disable\" - all cursors are set manually.\n"
    "2. \"Voltage\" - when manually changing the position of the cursor time cursors voltage automatically track changes in the signal.\n"
    "3. \"Time\" - when manually changing the position of the cursor voltage cursors time automatically track changes in the signal.\n"
    "4. \"Volt and time\" - acts as one of the previous modes, depending on which was carried out last effect cursors."
    ,
    DISABLE_RU, DISABLE_EN,
    "Напряжение",     "Voltage",
    "Время",          "Time",
    "Напряж и время", "Volt and time",
    CURS_LOOK_MODE(Chan::A), pageCursors, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4(       cLookModeChanB,                                                                               //--- КУРСОРЫ - Слежение канал 2 ---
    "Слежение \x8f, \x9f", "Tracking \x8f, \x9f"
    ,
    "Задаёт режим слежения за вторым курсором времени и напряжения:\n"
    "1. \"Откл\" - курсор времени и курсор напряжения устанавливаются вручную.\n"
    "2. \"Напряжение\" - при ручном изменении положения курсора времени курсор напряжения автоматически отслеживают изменения сигнала.\n"
    "3. \"Время\" - при ручном изменении положения курсора напряжения курсор времени автоматически отслеживают изменения сигнала.\n"
    "4. \"Напряж и время\" - действует как один из предыдущих режимов, в зависимости от того, на какой курсор производилось последнее воздействие."
    ,
    "Sets the mode tracking cursors channel 2:\n"
    "1. \"Disable\" - all cursors are set manually.\n"
    "2. \"Voltage\" - when manually changing the position of the cursor time cursors voltage automatically track changes in the signal.\n"
    "3. \"Time\" - when manually changing the position of the cursor voltage cursors time automatically track changes in the signal.\n"
    "4. \"Volt and time\" - acts as one of the previous modes, depending on which was carried out last effect cursors."
    ,
    DISABLE_RU,        DISABLE_EN,
    "Напряжение",      "Voltage",
    "Время",           "Time",
    "Напряж. и время", "Volt. and time",
    CURS_LOOK_MODE(Chan::B), pageCursors, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(       cShowFreq,                                                                                                //--- КУРОСРЫ - 1/dT ---
    "1/dT", "1/dT",
    "Если выбрано \"Вкл\", в правом верхнем углу выводится величина, обратная расстоянию между курсорами времени - частота сигнала, один период "
    "которого равен расстоянию между временными курсорами.",
    "If you select \"Enable\" in the upper right corner displays the inverse of the distance between cursors time - frequency signal, a period "
    "equal to the distance between the time cursors.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    CURSORS_SHOW_FREQ, pageCursors, FuncActive, FuncChangedChoice, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5(         pageCursors,                                                                                                      //--- ИЗМЕРЕНИЯ ---
    "КУРСОРЫ", "CURSORS",
    "Курсорные измерения",
    "Cursor measurements",
    &cShow,                                         ///< ИЗМЕРЕНИЯ - КУРСОРЫ - Показывать
    &cLookModeChanA,                                ///< ИЗМЕРЕНИЯ - КУРСОРЫ - Слежение канал 1
    &cLookModeChanB,                                ///< ИЗМЕРЕНИЯ - КУРСОРЫ - Слежение канал 2
    &cShowFreq,                                     ///< ИЗМЕРЕНИЯ - КУРОСРЫ - 1/dT
    PageMeasures::PageCursors::PageSet::pointer,    ///< ИЗМЕРЕНИЯ - КУРСОРЫ - УСТАНОВИТЬ
    Page::Name::Measures_Cursors, PageMeasures::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)
