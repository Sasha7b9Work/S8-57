#include "defines.h"
#include "Data/Reader.h"
#include "Display/Display_Types.h"
#include "Osci/Display/Accumulator.h"
#include "Osci/Display/Osci_Display.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  оличество отрисованных сигналов
static int numFrames = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ¬озвращает номер выводимого сигнала
static uint CurrentFrame();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Display::Accumulator::NextFrame()
{
    if (ACCUMULATION_IS_ENABLED)
    {
        static uint prevFrame = 0;

        LOG_WRITE("%d", CurrentFrame());

        if (CurrentFrame() == prevFrame)
        {
            return;
        }

        prevFrame = CurrentFrame();

        numFrames++;
        if (numFrames >= NUM_ACCUM)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint CurrentFrame()
{
    return (DATA) ? DATA->Number() : 0;
}
