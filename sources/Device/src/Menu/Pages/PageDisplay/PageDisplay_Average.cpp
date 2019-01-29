#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageAverage;

const PageBase *PageDisplay::PageAverage::pointer = &pageAverage;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_REG_10( cAverage_Num,                                                                                                                  //--- ДИСПЛЕЙ - УСРЕДНЕНИЕ - Количество ---
    "Количество", "Number",
    "Задаёт количество последних измерений, по которым производится усреднение.",
    "Sets number of the last measurements on which averaging is made.",
    DISABLE_RU, DISABLE_EN,
    "2", "2",
    "4", "4",
    "8", "8",
    "16", "16",
    "32", "32",
    "64", "64",
    "128", "128",
    "256", "256",
    "512", "512",
    ENUM_AVE, pageAverage, FuncActive, Choice::FuncChange, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cAverage_Mode,                                                                                                                           //--- ДИСПЛЕЙ - УСРЕДНЕНИЕ - Режим ---
    "Режим", "Mode"
    ,
    "1. \"Точно\" - точный режим усреднения, когда в расчёте участвуют только последние сигналы.\n"
    "2. \"Приблизительно\" - приблизительный режим усреднения. Имеет смысл использовать, когда задано количество измерений большее, чем может "
    "поместиться в памяти."
    ,
    "1. \"Accurately\" - the exact mode of averaging when only the last signals participate in calculation.\n"
    "2. \"Around\" - approximate mode of averaging. It makes sense to use when the number of measurements bigger is set, than can be located in "
    "memory."
    ,
    "Точно", "Accurately",
    "Приблизительно", "Around",
    MODE_AVE, pageAverage, FuncActive, Choice::FuncChange, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Average()
{
    return true;
}

DEF_PAGE_2( pageAverage, // -V641 // -V1027                                                                                                                    //--- ДИСПЛЕЙ - УСРЕДНЕНИЕ ---
    "УСРЕДН.", "AVERAGE",
    "Настройки режима усреднения по последним измерениям.",
    "Settings of the mode of averaging on the last measurements.",
    &cAverage_Num,         ///< ДИСПЛЕЙ - УСРЕДНЕНИЕ - Количество
    &cAverage_Mode,        ///< ДИСПЛЕЙ - УСРЕДНЕНИЕ - Режим
    Page::Name::Display_Average, PageDisplay::pointer, IsActive_Average, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
