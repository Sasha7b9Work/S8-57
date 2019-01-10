#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"
#include "Settings/SettingsTime.h"
#include "Utils/Measure.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Utils/Math.h"
#include "Hardware/Sound.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageTune;

const PageBase *PageMeasures::PageAuto::PageTune::pointer = &pageTune;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
DEF_SMALL_BUTTON_EXIT(bTune_Exit,                                                                            //--- ИЗМЕРЕНИЯ - НАСТРОИТЬ - Выход ---
    pageTune, FuncActive, OnPressSB_Exit, DrawSB_Exit
)
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Draw_Tune_Markers(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 2, '\x60');
    Painter::SetFont(Font::Type::_8);
}

DEF_SMALL_BUTTON(bTune_Markers,                                                                            //--- ИЗМЕРЕНИЯ - НАСТРОИТЬ - Маркер ---
    "Маркер", "Marker",
    "Позволяет установить маркеры для визуального контроля измерений",
    "Allows to establish markers for visual control of measurements",
    pageTune, FuncActive, Measure::ShortPressOnSmallButonMarker, Draw_Tune_Markers
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Tune_Settings()
{
    Measure::pageChoiceIsActive = !Measure::pageChoiceIsActive;
    if (Measure::pageChoiceIsActive)
    {
        Measure::posOnPageChoice = (int8)set.meas_measures[Measure::posActive];
    }
}

static void Draw_Tune_Settings(int x, int y)
{
    Painter::SetFont(Font::Type::_UGO2);
    Painter::Draw4SymbolsInRect(x + 2, y + 1, '\x62');
    Painter::SetFont(Font::Type::_8);
}

DEF_SMALL_BUTTON(bTune_Settings,                                                                        //--- ИЗМЕРЕНИЯ - НАСТРОИТЬ - Настройка ---
    "Настройка", "Setup",
    "Позволяет выбрать необходимые измерения",
    "Allows to choose necessary measurements",
    pageTune, FuncActive, OnPress_Tune_Settings, Draw_Tune_Settings
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool IsActive_Tune()
{
    return SHOW_MEASURES;
}

static bool HandlerKey_Tune(KeyEvent event)
{
    if (event.type != TypePress::Press)
    {
        return true;
    }

    Key::E key = event.key;

    int8 delta = (key == Key::Up || key == Key::Right) ? 1 : -1;

    if (Measure::pageChoiceIsActive)
    {
        Measure::posOnPageChoice += delta;
        Sound::RegulatorSwitchRotate();

        if (Measure::posOnPageChoice < 0)
        {
            Measure::posOnPageChoice = Measure::Type::Number - 1;
        }
        else if (Measure::posOnPageChoice == Measure::Type::Number)
        {
            Measure::posOnPageChoice = 0;
        }
        else
        {
            // здесь ничего делать не нужно
        }

        set.meas_measures[Measure::posActive] = (Measure::Type::E)Measure::posOnPageChoice;
        Painter::ResetFlash();
    }
    else
    {
        Measure::ChangeActive(delta);

        Sound::RegulatorSwitchRotate();
    }

    return true;
}

DEF_PAGE_2( pageTune, // -V641 // -V1027
    "НАСТРОИТЬ", "CONFIGURE",
    "Переход в режим точной настройки количества и видов измерений",
    "Transition to the fine tuning mode of the number and types of measurements",
    &bTune_Markers,
    &bTune_Settings,
    Page::Name::Measures_Auto_Tune, PageMeasures::PageAuto::pointer, IsActive_Tune, FuncPressPage, FuncDrawPage, HandlerKey_Tune
)
