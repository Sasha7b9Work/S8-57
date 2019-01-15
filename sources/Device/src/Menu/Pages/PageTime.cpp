#include "stdafx.h"
#ifndef WIN32
#include <stm32f4xx.h>
#include "defines.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageTime.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Menu/MenuItems.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include "Settings/Settings.h"
#endif


using namespace Osci::Settings;

extern const PageBase pTime;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Sample()
{
    return IN_RANDOM_MODE && !START_MODE_IS_SINGLE;
}

DEF_CHOICE_2( cSample,                                                                                                                                          //--- РАЗВЕРТКА - Выборка ---
    "Выборка", "Sampling",
    "\"Реальная\" - \n"
    "\"Эквивалентная\" -",
    "\"Real\" - \n"
    "\"Equals\" - ",
    "Реальное время", "Real",
    "Эквивалентная",  "Equals",
    SAMPLE_TYPE, pTime, IsActive_Sample, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_PeakDet()
{
    return (SET_TBASE >= TBase::MIN_PEAK_DET);
}

void PageTime::OnChanged_PeakDet(bool active)
{
    if (active)
    {
        PageMemory::OnChanged_Points(true);
    }
    else
    {
        Display::ShowWarning(Warning::TooSmallSweepForPeakDet);
    }
}

DEF_CHOICE_2( cPeakDet,                                                                                                                                         //--- РАЗВЕРТКА - Пик дет ---
    "Пик дет", "Pic deat",
    "Включает/выключает пиковый детектор.",
    "Turns on/off peak detector.",
    DISABLE_RU, DISABLE_EN,
    ENABLE_RU,  ENABLE_EN,
    SET_PEAKDET, pTime, IsActive_PeakDet, PageTime::OnChanged_PeakDet, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageTime::OnChanged_TPos(bool active)
{
    PageMemory::OnChanged_Points(active);
    TShift::Set(SET_TSHIFT);
}

DEF_CHOICE_3(cTPos,                                                                                                    //--- РАЗВЕРТКА - То ---
    "\x7b", "\x7b",
    "Задаёт точку привязки нулевого смещения по времени к экрану - левый край, центр, правый край.",
    "Sets the anchor point nuleovgo time offset to the screen - the left edge, center, right edge.",
    "Лево", "Left",
    "Центр", "Center",
    "Право", "Right",
    TPOS, pTime, FuncActive, PageTime::OnChanged_TPos, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2(cDivRole,                                                                                       //--- РАЗВЕРТКА - Ф-ция ВР/ДЕЛ ---
    "Ф-ция ВР/ДЕЛ", "Funct Time/DIV"
    ,
    "Задаёт функцию для ручки ВРЕМЯ/ДЕЛ: в режиме сбора информации (ПУСК/СТОП в положении ПУСК):\n"
    "1. \"Время\" - изменение смещения по времени.\n"
    "2. \"Память\" - перемещение по памяти."
    ,
    "Sets the function to handle TIME/DIV: mode of collecting information (START/STOP to start position):\n"
    "1. \"Time\" - change the time shift.\n"
    "2. \"Memory\" - moving from memory."
    ,
    "Время", "Time",
    "Память", "Memory",
    TIME_DIV_XPOS, pTime, FuncActive, FuncChangedChoice, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cShiftXtype,                                                                                                                                     //--- РАЗВЕРТКА - Смещение ---
    "Смещение", "Offset"
    ,
    "Задаёт режим удержания смещения по горизонтали\n1. \"Время\" - сохраняется абсолютное смещение в секундах.\n2. \"Деления\" - сохраняется "
    "положение мещения на экране."
    ,
    "Sets the mode of retaining the horizontal displacement\n1. \"Time\" - saved the asbolutic offset in seconds.\n2. \"Divisions\" - retained "
    "the position of the offset on the screen."
    ,
    "Время", "Time",
    "Деления", "Divisions",
    LINKING_TSHIFT, pTime, FuncActive, FuncChangedChoice, FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageTime::pointer = &pTime;


DEF_PAGE_5( pTime, // -V641                                                                                                                                               //--- РАЗВЕРТКА ---
    "РАЗВ.", "SCAN",
    "Содержит настройки развёртки.",
    "Contains scan settings.",
    &cSample,        // РАЗВЕРТКА - Выборка
    &cPeakDet,       // РАЗВЕРТКА - Пик дет
    &cTPos,          // РАЗВЕРТКА - То
    &cDivRole,       // РАЗВЕРТКА - Ф-ция ВР/ДЕЛ
    &cShiftXtype,    // РАЗВЕРТКА - Смещение
    Page::Name::Time, Menu::pageMain, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
