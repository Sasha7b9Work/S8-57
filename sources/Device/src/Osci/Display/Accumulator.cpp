// 2023/11/22 13:47:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"



// Количество отрисованных сигналов
static int numFrames = 0;



// Возвращает номер выводимого сигнала
static uint CurrentFrame();



void DisplayOsci::Accumulator::NextFrame()
{
    if (S_DISP_NUM_ACCUM > 1)
    {
        if (S_DISP_ENUM_ACCUM_IS_INFINITY)
        {
            return;
        }

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
