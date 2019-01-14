#include "defines.h"
#include "Settings.h"
#include "SettingsDebug.h"


using FPGA::SET::Range;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float GetStretchADC(Chan::E ch)
{
    static const int16 *addStretch[Range::Number][2] =
    {
        {0, 0},  // 2mV
        {0, 0},  // 5mV
        {0, 0},  // 10mV
        {0, 0},
        {0, 0},
        {0, 0},
        /*
        {&NRST_ADD_STRETCH_20mV_A, &NRST_ADD_STRETCH_20mV_B},   // 20mV
        {&NRST_ADD_STRETCH_50mV_A, &NRST_ADD_STRETCH_50mV_B},   // 50mV
        {&NRST_ADD_STRETCH_100mV_A, &NRST_ADD_STRETCH_100mV_B}, // 100mV
        */
        {0, 0}, // 200mV
        {0, 0}, // 500mV
        {0, 0},
        {0, 0},
        {0, 0}
        /*
        {&NRST_ADD_STRETCH_20mV_A, &NRST_ADD_STRETCH_20mV_B},   // 1V
        {&NRST_ADD_STRETCH_2V_A, &NRST_ADD_STRETCH_2V_B},       // 2V
        {&NRST_ADD_STRETCH_100mV_A, &NRST_ADD_STRETCH_100mV_B}  // 5V
        */
    };
    
    const int16 *address = addStretch[SET_RANGE(ch)][ch];

    int16 stretch = NRST_STRETCH_ADC(ch, NRST_STRETCH_ADC_TYPE);

    if (address)
    {
        stretch += (*address);
    }

    return stretch * 1e-4F + 1.0F;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void SetStretchADC(Chan::E ch, float kStretch)
{
    NRST_STRETCH_ADC(ch, NRST_STRETCH_ADC_TYPE) = (int16)((kStretch - 1.0F) * 1e4F);
}
