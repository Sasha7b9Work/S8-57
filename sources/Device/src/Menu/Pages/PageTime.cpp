#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Utils/Math.h"


bool SampleType::IsReal()
{
    if (!OSCI_IN_MODE_RANDOMIZER || S_TRIG_START_MODE_IS_SINGLE)
    {
        return true; 
    }

    return (S_RAND_SAMPLE_TYPE == SampleType::Real);
}


int TPos::PosX()
{
    int x[] = { Grid::Left(), (Grid::Right() - Grid::Left()) / 2 + Grid::Left(), Grid::Right() };

    return x[S_TPOS] - DisplayOsci::ShiftInMemory::Get() + DisplayOsci::ShiftInMemory::Default(S_TPOS);
}


void TPos::Set(TPos::E v)
{
    S_TPOS = v;
    PageTime::OnChanged_TPos(true);
}


void TPos::Draw()
{
    int x0 = PosX();

    if (x0 < Grid::Left() || x0 > Grid::Right())
    {
        return;
    }

    x0 -= 3;

    int y = Grid::Top() - 1;
    
    Region(6, 6).Fill(x0, y, Color::BACK);

    Char(Symbol8::TPOS_1).Draw(x0, y, Color::FILL);
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
    return m[S_ENUMPOINTS_FPGA][S_PEAK_DET][S_TPOS];
}


bool PeakDetMode::IsEnabled()
{
    return (S_PEAK_DET_ENABLED && (S_TIME_BASE >= TBase::MIN_PEAK_DET));
}


static bool IsActive_Sample()
{
    return OSCI_IN_MODE_RANDOMIZER && (!S_TRIG_START_MODE_IS_SINGLE);
}

DEF_CHOICE_2( cSample,                                                                                                                                          //--- РАЗВЕРТКА - Выборка ---
    "Выборка"
    ,
    "\"Реальная\" - \n"
    "\"Эквивалентная\" -"
    ,
    "Эквивалентная",
    "Реальное время",
    S_RAND_SAMPLE_TYPE, &PageTime::self, IsActive_Sample, Choice::Changed, Choice::AfterDraw
)

void PageTime::SetSampleTime(SampleType::E v)
{
    if (IsActive_Sample())
    {
        S_RAND_SAMPLE_TYPE = v;
    }
}


static bool IsActive_PeakDet()
{
    return (S_TIME_BASE >= TBase::MIN_PEAK_DET);
}

void PageTime::OnChanged_PeakDet(bool active)
{
    if (active)
    {
        Osci::OnChangedLengthMemoryChannel();
    }
    else
    {
        Display::ShowWarning("Пик. Дет. не работает на развёртках менее 5 мкс/дел");
    }
}

DEF_CHOICE_2( cPeakDet,                                                                                                                                         //--- РАЗВЕРТКА - Пик дет ---
    "Пик дет",
    "Включает/выключает пиковый детектор."
    ,
    DISABLE_RU,
    ENABLE_RU,
    S_PEAK_DET, &PageTime::self, IsActive_PeakDet, PageTime::OnChanged_PeakDet, Choice::AfterDraw
)

void PeakDetMode::Set(PeakDetMode::E v)
{
    S_PEAK_DET = v;

    PageTime::OnChanged_PeakDet(cPeakDet.IsActive());
}

void PageTime::OnChanged_TPos(bool active)
{
    PageMemory::OnChanged_LengthMemoryChannel(active);
    TShift::Set(S_TIME_SHIFT);
}

DEF_CHOICE_3( cTPos,                                                                                                                                                 //--- РАЗВЕРТКА - То ---
    "\x7b",
    "Задаёт точку привязки нулевого смещения по времени к экрану - левый край, центр, правый край."
    ,
    "Лево",
    "Центр",
    "Право",
    S_TPOS, &PageTime::self, Item::Active, PageTime::OnChanged_TPos, Choice::AfterDraw
)


DEF_CHOICE_2( cShiftXtype,                                                                                                                                     //--- РАЗВЕРТКА - Смещение ---
    "Смещение",
    "Задаёт режим удержания смещения по горизонтали\n1. \"Время\" - сохраняется абсолютное смещение в секундах.\n2. \"Деления\" - сохраняется положение мещения на экране."
    ,
    "Время",
    "Деления",
    S_LINKING_TSHIFT, &PageTime::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

DEF_PAGE_3( pTime,                                                                                                                                                        //--- РАЗВЕРТКА ---
    "РАЗВ",
    "Содержит настройки развёртки",
    &cPeakDet,
    &cSample,
    &cTPos,
//  &cShiftXtype,
    PageName::Time, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageTime::self = static_cast<const Page *>(&pTime);
