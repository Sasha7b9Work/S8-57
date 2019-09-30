#include "defines.h"
#include "Settings.h"



enum Parameter
{
    DisplayModeDrawSignal = 8,
    DisplayThicknessSignal,
    DisplayBackground,
    DisplayShowStringNavigation,
    DisplayENumMinMax,
    DisplayModeAveraging,
    DisplayENumAverage,
    DisplayENumAccum,
    DisplayModeA
};

static uint8 *data;


void Settings::SetData(uint8 d[1024])
{
    data = d;
}


void Settings::SaveEnum16(uint8 name, uint8 value)
{
    *data++ = name;
    *data++ = value;
}
