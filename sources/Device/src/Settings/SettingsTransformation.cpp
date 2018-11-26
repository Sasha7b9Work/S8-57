#include "defines.h"
#include "Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::TransformForSave(uint8 d[1024])
{
    SetData(d);

    //SaveEnum16(DisplayModeDrawSignal, disp_modeDrawSignal);
    
    //*data++ = DisplayModeDrawSignal;
    //*data++ = disp_modeDrawSignal;

    *((uint16 *)data) = (uint16)((DisplayModeDrawSignal | (disp_modeDrawSignal << 8)));
    data += 2;
    
    SaveEnum16(DisplayThicknessSignal, disp_thickness);
    
    //*data++ = DisplayThicknessSignal;
    //*data++ = disp_thickness;
    
    SaveEnum16(DisplayBackground, (uint8)disp_background);
    
    //*data++ = DisplayBackground;
    //*data++ = disp_background;
    
    SaveEnum16(DisplayShowStringNavigation, set.disp_showStringNavigation);
    SaveEnum16(DisplayENumMinMax, set.disp_ENumMinMax);
    SaveEnum16(DisplayModeAveraging, set.disp_modeAveraging);
    SaveEnum16(DisplayENumAverage, set.disp_ENumAverage);
    SaveEnum16(DisplayENumAccum, set.disp_ENumAccum);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::TransformFromLoad(uint8 /*d*/ [1024])
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::SetData(uint8 d[1024])
{
    data = d;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::SaveEnum16(uint8 name, uint8 value)
{
    *data++ = name;
    *data++ = value;
}
