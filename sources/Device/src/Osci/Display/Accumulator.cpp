#include "defines.h"
#include "Data/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"



///  оличество отрисованных сигналов
static int numFrames = 0;



/// ¬озвращает номер выводимого сигнала
static uint CurrentFrame();



void Osci::Display::Accumulator::NextFrame()
{
    if (set.disp.modeAccumulation == ModeAccumulation::Reset)
    {
        static uint prevFrame = 0;

        LOG_WRITE("%d", CurrentFrame());

        if (CurrentFrame() == prevFrame)
        {
            return;
        }

        prevFrame = CurrentFrame();

        numFrames++;
        if (numFrames >= set.disp.ENumAccum)
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


void Osci::Display::Accumulator::Reset()
{
    numFrames = 0;
}


static uint CurrentFrame()
{
    return (DATA) ? DATA->Number() : 0;
}
