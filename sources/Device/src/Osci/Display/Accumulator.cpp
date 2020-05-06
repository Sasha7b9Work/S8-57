#include "defines.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"



//  оличество отрисованных сигналов
static int numFrames = 0;



// ¬озвращает номер выводимого сигнала
static uint CurrentFrame();



void DisplayOsci::Accumulator::NextFrame()
{
    if (S_DISP_MODE_ACCUM_IS_RESET)
    {
        static uint prevFrame = 0;

        if (CurrentFrame() == prevFrame)
        {
            return;
        }

        prevFrame = CurrentFrame();

        numFrames++;
        if (numFrames >= S_DISP_NUM_ACCUM)
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
