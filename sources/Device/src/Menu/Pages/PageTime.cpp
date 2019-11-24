#include "defines.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageTime.h"
#include "FPGA/FPGA.h"
#include "Menu/MenuItems.h"
#include "Utils/CommonFunctions.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"


TPos::operator TPos::E()
{
    return set.time.tPos;
}


PeakDetMode::operator PeakDetMode::E()
{
    return set.time.peakDet;
}


SampleType::operator SampleType::E()
{
    return set.time.sampleType;
}


void SampleType::Set(SampleType::E type)
{
    set.time.sampleType = type;
}


int TPos::InBytes()
{
    static const int m[][2][3] =
    {
        {{0,   256,  510},  {0,   512,  1022}},
        {{0,   512,  1022}, {0,  1024,  2046}},
        {{0,  1024,  2046}, {0,  2048,  4094}},
        {{0,  2048,  4094}, {0,  4096,  8190}},
        {{0,  4096,  8190}, {0,  8192, 16382}},
        {{0,  8192, 16382}, {0,  8192, 16382}},
        {{0, 16384, 32766}, {0, 16384, 32766}}
    };
    return m[ENumPointsFPGA()][PeakDetMode()][set.time.tPos];
}


bool PeakDetMode::IsEnabled() const
{
    return (set.time.peakDet == PeakDetMode::Enabled) && (TBase() >= TBase::MIN_PEAK_DET);
}


static bool IsActive_Sample()
{
    return Osci::InModeRandomizer() && (set.trig.startMode != TrigStartMode::Single);
}

DEF_CHOICE_2( cSample,                                                                                                                                          //--- РАЗВЕРТКА - Выборка ---
    "Выборка"
    ,
    "\"Реальная\" - \n"
    "\"Эквивалентная\" -"
    ,
    "Реальное время",
    "Эквивалентная",
    set.time.sampleType, &PageTime::self, IsActive_Sample, 0, 0
)


static bool IsActive_PeakDet()
{
    return (TBase() >= TBase::MIN_PEAK_DET);
}

void PageTime::OnChanged_PeakDet(bool active)
{
    if (active)
    {
        Osci::OnChangedPoints();
    }
    else
    {
        Display::ShowWarning("Пик. Дет. не работает на развёртках менее 0.5мкс/дел");
    }
}

DEF_CHOICE_2( cPeakDet,                                                                                                                                         //--- РАЗВЕРТКА - Пик дет ---
    "Пик дет",
    "Включает/выключает пиковый детектор."
    ,
    DISABLE_RU,
    ENABLE_RU,
    set.time.peakDet, &PageTime::self, IsActive_PeakDet, PageTime::OnChanged_PeakDet, Choice::AfterDraw
)


void PageTime::OnChanged_TPos(bool active)
{
    PageMemory::OnChanged_Points(active);
    TShift(set.time.shift);
}

DEF_CHOICE_3( cTPos,                                                                                                                                                 //--- РАЗВЕРТКА - То ---
    "\x7b",
    "Задаёт точку привязки нулевого смещения по времени к экрану - левый край, центр, правый край."
    ,
    "Лево",
    "Центр",
    "Право",
    set.time.tPos, &PageTime::self, Item::Active, PageTime::OnChanged_TPos, Choice::AfterDraw
)


//_DEF_CHOICE_2(cDivRole,                                                                                       //--- РАЗВЕРТКА - Ф-ция ВР/ДЕЛ ---
//    "Ф-ция ВР/ДЕЛ"
//    ,
//    "Задаёт функцию для ручки ВРЕМЯ/ДЕЛ: в режиме сбора информации (ПУСК/СТОП в положении ПУСК):\n"
//    "1. \"Время\" - изменение смещения по времени.\n"
//    "2. \"Память\" - перемещение по памяти."
//    ,
//    "Время",
//    "Память",
//    TIME_DIV_XPOS, PageTime::self, 0, 0, 0
//)


DEF_CHOICE_2( cShiftXtype,                                                                                                                                     //--- РАЗВЕРТКА - Смещение ---
    "Смещение",
    "Задаёт режим удержания смещения по горизонтали\n1. \"Время\" - сохраняется абсолютное смещение в секундах.\n2. \"Деления\" - сохраняется положение мещения на экране."
    ,
    "Время",
    "Деления",
    set.time.linkingTShift, &PageTime::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

DEF_PAGE_4( pTime,                                                                                                                                                        //--- РАЗВЕРТКА ---
    "РАЗВ",
    "Содержит настройки развёртки",
    &cSample,
    &cPeakDet,
    &cTPos,
    //&cDivRole,
    &cShiftXtype,
    PageName::Time, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageTime::self = static_cast<const Page *>(&pTime);
