#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


bool MeasuresModeViewSignals::IsCompress()
{
    return (set.meas.modeViewSignals == MeasuresModeViewSignals::Compress);
}


bool MeasuresOnDisplay::IsVertical()
{
    return (set.meas.number == MeasuresOnDisplay::_6_1) || (set.meas.number == MeasuresOnDisplay::_6_2);
}


CursorsControl::E &CursorsControl::RefU(Chan::E ch)
{
    return S_CURS_CONTROL_U(ch);
}


CursorsControl::E &CursorsControl::RefT(Chan::E ch)
{
    return S_CURS_CONTROL_T(ch);
}


bool CursorsControl::IsDisabledT()
{
    return RefT(S_CURS_SOURCE) == Disabled;
}


bool CursorsControl::IsEnabled1T()
{
    return (RefT(S_CURS_SOURCE) == _1) || (RefT(S_CURS_SOURCE) == Both);
}


bool CursorsControl::IsEnabled2T()
{
    return (RefT(S_CURS_SOURCE) == _2) || (RefT(S_CURS_SOURCE) == Both);
}


bool CursorsControl::IsDisabledU()
{
    return RefU(S_CURS_SOURCE) == Disabled;
}


bool CursorsControl::IsEnabled1U()
{
    return (RefU(S_CURS_SOURCE) == _1) || (RefU(S_CURS_SOURCE) == Both);
}


bool CursorsControl::IsEnabled2U()
{
    return (RefU(S_CURS_SOURCE) == _2) || (RefU(S_CURS_SOURCE) == Both);
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
