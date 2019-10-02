#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Symbols.h"
#include "Menu/MenuItems.h"
#include "Osci/Measurements/Measurements.h"
#include "Settings/Settings.h"


static void Draw_Markers(int x, int y)
{
    Char(SymbolUGO2::BIG_M).Draw4SymbolsInRect(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bMarkers,                                                                                                                          //--- ИЗМЕРЕНИЯ - НАСТРОИТЬ - Маркер ---
    "Маркер",
    "Позволяет установить маркеры для визуального контроля измерений",
    &PageAutoMeasures::PageTune::self, Item::Active, Measure::ShortPressOnSmallButonMarker, Draw_Markers
)


static void Draw_Settings(int x, int y)
{
    Char(SymbolUGO2::SETTINGS).Draw4SymbolsInRect(x + 2, y + 1);
}

DEF_GRAPH_BUTTON( bSettings,                                                                                                                      //--- ИЗМЕРЕНИЯ - НАСТРОИТЬ - Настройка ---
    "Настройка",
    "Позволяет выбрать необходимые измерения",
    &PageAutoMeasures::PageTune::self, Item::Active, Osci::Measurements::PageChoice::ChangeState, Draw_Settings
)


static bool IsActive_Tune()
{
    return set.meas.show;
}

static bool HandlerKey_Tune(const KeyEvent &event)
{
    if (event.IsArrow())
    {
        Osci::Measurements::PageChoice::OnKeyEvent(event);
        return true;
    }

    return false;
}

DEF_PAGE_2( pTune, // -V641 // -V1027
    "НАСТРОИТЬ",
    "Переход в режим точной настройки количества и видов измерений",
    &bMarkers,
    &bSettings,
    PageName::AutoMeasures_Tune, &PageAutoMeasures::self, IsActive_Tune, Page::OpenClose, Page::BeforeDraw, HandlerKey_Tune
)

const Page *const PageAutoMeasures::PageTune::self = (const Page *)&pTune;
