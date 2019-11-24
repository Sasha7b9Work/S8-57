#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


MeasuresOnDisplay::operator MeasuresOnDisplay::E()
{
    return set.meas.number;
}


bool MeasuresSource::IsA() const
{
    return (set.meas.source == MeasuresSource::A);
}


bool MeasuresSource::IsB() const
{
    return (set.meas.source == MeasuresSource::B);
}


bool MeasuresSource::IsBoth() const
{
    return (set.meas.source == MeasuresSource::A_B);
}


bool MeasuresModeViewSignals::IsCompress() const
{
    return (set.meas.modeViewSignals == MeasuresModeViewSignals::Compress);
}


bool MeasuresOnDisplay::IsVertical() const
{
    return (set.meas.number == MeasuresOnDisplay::_6_1) || (set.meas.number == MeasuresOnDisplay::_6_2);
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
