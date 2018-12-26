#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageDebug.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageRand;

const PageBase *PageDebug::PageRand::pointer = &pageRand;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(gRand_NumAverage,                                                                           //--- ОТЛАДКА - РАНД-ТОР - Усредн. ---
    "Усредн.", "Average",
    "",
    "",
    NRST_NUM_AVE_FOR_RAND, 1, 32, pageRand, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(gRand_NumSmooth,                                                                        //--- ОТЛАДКА - РАНД-ТОР - Сглаживание ---
    "Сглаживание", "Smoothing",
    "",
    "",
    NRST_NUM_SMOOTH_FOR_RAND, 1, 10, pageRand, FuncActive, FuncChanged, FuncBeforeDraw
)

static void OnChanged_Rand_NumMeasures()
{
    FPGA::SetNumberMeasuresForGates(NUM_MEASURES_FOR_GATES);
}

DEF_GOVERNOR(gRand_NumMeasures,                                                                     //--- ОТЛАДКА - РАНД-ТОР - Выб-к/ворота ---
    "Выб-к/ворота", "Samples/gates",
    "",
    "",
    NUM_MEASURES_FOR_GATES, 1, 2500, pageRand, FuncActive, OnChanged_Rand_NumMeasures, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cRand_ShowInfo,                                                                          //--- ОТЛАДКА - РАНД-ТОР - Информация ---
    "Информация", "Information",
    "Показывать информацию о воротах рандомизатора",
    "To show information on randomizer gate",
    "Не показывать", "Hide",
    "Показывать", "Show",
    SHOW_RAND_INFO, pageRand, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(gRand_ShowStat,                                                                          //--- ОТЛАДКА - РАНД-ТОР - Статистика ---
    "Статистика", "Statistics",
    "Показывать график статистики",
    "Statistics show schedule",
    "Не показывать", "Hide",
    "Показывать", "Show",
    SHOW_RAND_STAT, pageRand, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Rand_TimeCompensation()
{
    FPGA::SetDeltaTShift(TIME_COMPENSATION);
}

DEF_GOVERNOR(gRand_TimeCompensation,                                                            //--- ОТЛАДКА - РАНД-ТОР - Компенсация задержки ---
    "Компенсация задержки", "Compenstaion time",
    "Подстройка компенсации задержки АЦП 40 нс",
    "",
    TIME_COMPENSATION, 0, 510, pageRand, FuncActive, OnChanged_Rand_TimeCompensation, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Rand_AddTimeShift()
{
    FPGA::SetTShift(SET_TSHIFT);
}

static int16 addShift = 0;

DEF_GOVERNOR(gRand_AddTimeShift,                                                                        //--- ОТЛАДКА - РАНД-ТОР - Смещение ---
    "Доп смещение", "Add shift",
    "Добавочное смщение при вращении tShift",
    "",
    addShift, -100, 100, pageRand, FuncActive, OnChanged_Rand_AddTimeShift, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Rand_Pretriggered()
{
    FPGA::hardware.LoadTShift();
}

DEF_GOVERNOR(gRand_Pretriggered,                                                                      //--- ОТЛАДКА - РАНД-ТОР - Предзапуск ---
    "Предзапуск", "Pretiggered",
    "Величина предзапуска, которая пишется в рандомизатор",
    "",
    PRETRIGGERED, 0, 30000, pageRand, FuncActive, OnChanged_Rand_Pretriggered, FuncBeforeDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_16( gGateMax,                                                                                                                           //--- ОТЛАДКА - РАНД-ТОР - Ворота верх ---
    "Ворота верх", "Gates up",
    "Устанавливает учитываемое расстояние от максимума ворот для рандомизатора",
    "",
    "0",  "0",
    "10", "10",
    "20", "20",
    "30", "30",
    "40", "40",
    "50", "50",
    "60", "60",
    "70", "70",
    "80", "80",
    "90", "90",
    "100", "100",
    "110", "110",
    "120", "120",
    "130", "130",
    "140", "140",
    "150", "150",
    set.dbg_enum_gate_max, pageRand, FuncActive, FuncChangedChoice, FuncDraw
)


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_16( gGateMin,                                                                                                                            //--- ОТЛАДКА - РАНД-ТОР - Ворота низ ---
    "Ворота низ", "Gates down",
    "Устанавливает учитываемое расстояние от минимума ворот для рандомизатора",
    "",
    "0", "0",
    "10", "10",
    "20", "20",
    "30", "30",
    "40", "40",
    "50", "50",
    "60", "60",
    "70", "70",
    "80", "80",
    "90", "90",
    "100", "100",
    "110", "110",
    "120", "120",
    "130", "130",
    "140", "140",
    "150", "150",
    set.dbg_enum_gate_min, pageRand, FuncActive, FuncChangedChoice, FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageRand, // -V641 // -V1027                                                                                                                         //--- ОТЛАДКА - РАНД-ТОР ---
    "РАНД-ТОР", "RANDOMIZER",
    "",
    "",
    &gGateMin,                      ///< ОТЛАДКА - РАНД-ТОР - Ворота низ
    &gGateMax,                      ///< ОТЛАДКА - РАНД-ТОР - Ворота верх

    //&gRand_NumAverage,          ///< ОТЛАДКА - РАНД-ТОР - Усредн.
    //&gRand_NumSmooth,           ///< ОТЛАДКА - РАНД-ТОР - Сглаживание
    //&gRand_NumMeasures,         ///< ОТЛАДКА - РАНД-ТОР - Выб-к/ворота
    //&cRand_ShowInfo,            ///< ОТЛАДКА - РАНД-ТОР - Информация
    //&gRand_ShowStat,            ///< ОТЛАДКА - РАНД-ТОР - Статистика
    //&gRand_TimeCompensation,    ///< ОТЛАДКА - РАНД-ТОР - Компенсация задержки
    //&gRand_AddTimeShift,        ///< ОТЛАДКА - РАНД-ТОР - Смещение
    //&gRand_Pretriggered,        ///< ОТЛАДКА - РAНД-ТОР - Предзапуск
    Page::Name::Debug_Rand, PageDebug::pointer, FuncActive, EmptyPressPage, FuncDrawPage, FuncRegSetPage
)
