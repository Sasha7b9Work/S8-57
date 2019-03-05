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

    const float ampl = 25.0F;    // Амплитуда шума

    float min = prev - ampl;
    if (min < -ampl)
    {
        min = -ampl;
    }

    float max = prev + ampl;
    if (max > ampl)
    {
        max = ampl;
    }

    prev = Math::RandFloat(min, max);

    return prev;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadDataChanenl(Chan::E ch, uint8 data[MAX_NUM_POINTS])
{
    std::memset(data, (ch == Chan::A) ? (FPGA::VALUE::AVE) : (FPGA::VALUE::AVE + 10), FPGA_NUM_POINTS);

    float amplitude = 100.0F;

    for (uint i = 0; i < FPGA_NUM_POINTS; i++)
    {
        float noise = 5.0F * std::rand() / RAND_MAX ;

        data[i] = (uint8)(FPGA::VALUE::AVE + amplitude * (sinf(i * 0.1F)) + NextNoise());
    }
}
