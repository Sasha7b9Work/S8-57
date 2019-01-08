#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "DisplayPrimitives.h"
#include "Display/Painter.h"
#include "Utils/String.h"
#include <stdio.h>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ProgressBar_Draw(const ProgressBar *bar)
{
    int x = bar->x;
    int y = bar->y;
    float passedPercents = bar->fullTime == 0.0F ? 0 : bar->passedTime / bar->fullTime * 100; //-V550
    Painter::DrawStringInCenterRect(x, y - 15, bar->width, bar->height, String("Завершено %.1f %%", (float)passedPercents).CString(), Color::FILL);
    Painter::DrawRectangle(bar->x, bar->y, bar->width, bar->height);
    Painter::FillRegion(bar->x, bar->y, (int)(bar->width * passedPercents / 100.0F), bar->height);
    Painter::DrawStringInCenterRect(x, y + bar->height, bar->width, bar->height, String("Осталось %.1f с", (int)(bar->fullTime - bar->passedTime) / 1000.0F).CString());
}
