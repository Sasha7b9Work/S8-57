#include "defines.h"
#include "Measurements.h"

#include "Settings/Settings.h"
#include "Hardware/Beeper.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool isActive = false;
int8 Osci::Measurements::PageChoice::posCursor = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Osci::Measurements::PageChoice::IsActive()
{
    return isActive;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Measurements::PageChoice::ChangeState()
{
    isActive = !isActive;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Measurements::PageChoice::OnOpenCloseEvent()
{
    ChangeState();
    if (IsActive())
    {
        posCursor = (int8)set.meas_measures[Osci::Measurements::posActive];
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Measurements::PageChoice::OnKeyEvent(KeyEvent event)
{
    if (event.type != TypePress::Press)
    {
        return;
    }

    Key::E key = event.key;

    int8 delta = (key == Key::Up || key == Key::Right) ? 1 : -1;

    if (Measurements::PageChoice::IsActive())
    {
        posCursor += delta;
        Beeper::RegulatorSwitchRotate();

        if (posCursor < 0)
        {
            posCursor = Measure::Type::Number - 1;
        }
        else if (posCursor == Measure::Type::Number)
        {
            posCursor = 0;
        }
        else
        {
            // здесь ничего делать не нужно
        }

        set.meas_measures[Osci::Measurements::posActive] = (Measure::Type::E)posCursor;
        Color::ResetFlash();
    }
    else
    {
        Measure::ChangeActive(delta);

        Beeper::RegulatorSwitchRotate();
    }
}
