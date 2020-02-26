#include "defines.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"



///  оличество отрисованных сигналов
static int numFrames = 0;



/// ¬озвращает номер выводимого сигнала
static uint CurrentFrame();



void DisplayOsci::Accumulator::NextFrame()
{
    if (ModeAccumulation::IsReset())
    {
        static uint prevFrame = 0;

//        LOG_WRITE("%d", CurrentFrame());

        if (CurrentFrame() == prevFrame)
        {
            return;
        }

        prevFrame = CurrentFrame();

        numFrames++;
        if (numFrames >= ENumAccum())
        {
            DisplayOsci::SetFlagRedraw();
            numFrames = 0;
        }
    }
    else
    {
        DisplayOsci::SetFlagRedraw();
    }
}


void DisplayOsci::Accumulator::Reset()
{
    numFrames = 0;
}


static uint CurrentFrame()
{
    return (DS) ? DS->id : 0;
}
