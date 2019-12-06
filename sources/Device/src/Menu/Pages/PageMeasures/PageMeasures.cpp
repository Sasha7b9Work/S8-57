#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


MeasuresOnDisplay::operator MeasuresOnDisplay::E()
{
    return set.meas.number;
}


bool MeasuresSource::IsA()
{
    return (set.meas.source == MeasuresSource::A);
}


bool MeasuresSource::IsB()
{
    return (set.meas.source == MeasuresSource::B);
}


bool MeasuresSource::IsBoth()
{
    return (set.meas.source == MeasuresSource::A_B);
}


bool MeasuresModeViewSignals::IsCompress()
{
    return (set.meas.modeViewSignals == MeasuresModeViewSignals::Compress);
}


bool MeasuresOnDisplay::IsVertical()
{
    return (set.meas.number == MeasuresOnDisplay::_6_1) || (set.meas.number == MeasuresOnDisplay::_6_2);
}


CursorsActive::E &CursorsActive::Ref()
{
    return set.curs.active;
}


CursorsLookMode::E &CursorsLookMode::Ref(Chan::E ch)
{
    return set.curs.lookMode[ch];
}

bool CursorsMovement::IsPercents()
{
    return (set.curs.movement == Percents);
}


bool CursorsMovement::IsPixels()
{
    return (set.curs.movement == Pixels);
}


Chan::E &CursorsSource::Ref()
{
    return set.curs.source;
}


CursorsControl::E &CursorsControl::RefU(Chan::E ch)
{
    return set.curs.cntrlU[ch];
}


CursorsControl::E &CursorsControl::RefT(Chan::E ch)
{
    return set.curs.cntrlT[ch];
}


bool CursorsControl::IsDisabledT()
{
    return (set.curs.cntrlT[CursorsSource()] == Disabled);
}


bool CursorsControl::IsEnabled1T()
{
    return (set.curs.cntrlT[CursorsSource()] == _1) || (set.curs.cntrlT[CursorsSource()] == Both);
}


bool CursorsControl::IsEnabled2T()
{
    return (set.curs.cntrlT[CursorsSource()] == _2) || (set.curs.cntrlT[CursorsSource()] == Both);
}


DEF_PAGE_3( pMeasures,                                                                                                                                                    //--- ИЗМЕРЕНИЯ ---
    "ИЗМЕРЕНИЯ",
    "Доступ к настройкам измерений - курсорных и автоматических",
    PageCursorsMeasures::self,
    PageAutoMeasures::self,
    PageMath::self,
    PageName::Measures, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageMeasures::self = static_cast<const Page *>(&pMeasures);
