#include "defines.h"
#include "FPGA/FPGA.h"
#include <cstring>
#include "Settings/Settings.h"
#include <cstdlib>
#include "Utils/Math.h"


uint16 addrRead = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::Init()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static float NextNoise()
{
    static float prev = 0.0F;   // Предыдущее значение шума

    const float ampl = 20.0F;    // Амплитуда шума

    const float step = 7.0F;

    float min = prev - step;
    LIMIT_BELOW(min, -ampl);

    float max = prev + step;
    LIMIT_ABOVE(max, ampl);

    prev = Math::RandFloat(min, max);

    return prev;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadDataChanenl(Chan::E ch, uint8 data[MAX_NUM_POINTS])
{
    float amplitude = 100.0F;

    for (uint i = 0; i < FPGA_NUM_POINTS; i++)
    {
        data[i] = (uint8)(FPGA::VALUE::AVE + amplitude * (sinf(i * 0.1F)) + NextNoise());
    }
}
