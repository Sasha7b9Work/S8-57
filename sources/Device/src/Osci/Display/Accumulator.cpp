#include "defines.h"
#include "Display/Display_Types.h"
#include "Osci/Display/Accumulator.h"
#include "Osci/Display/Osci_Display.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Количество отрисованных сигналов
static int numFrames = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Display::Accumulator::NextDraw()
{
    if (ACCUMULATION_IS_ENABLED)
    {
        numFrames++;
        if (numFrames > NUM_ACCUM)
        {
            Osci::Display::SetFlagRedraw();
            numFrames = 0;
        }
    }
    else
    {
        Osci::Display::SetFlagRedraw();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Display::Accumulator::Reset()
{
    numFrames = 0;
}
