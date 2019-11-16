#include "defines.h"
#include "FPGA/FPGA.h"
#include "GUI/Dialogs/TuneGeneratorDialog.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <cstdlib>
#include <cstring>


uint16 addrRead = 0;



void FPGA::Init()
{
}


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


bool FPGA::ReadDataChanenl(Chan::E ch, uint8 data[MAX_NUM_POINTS])
{
    if (!set.ch[ch].enabled)
    {
        return false;
    }

    float amplitude = 100.0F * TuneGeneratorDialog::amplitude;
    float offset = 0.0F + TuneGeneratorDialog::offset;
    float frequency = TuneGeneratorDialog::frequency;

    for (uint i = 0; i < FPGA_NUM_POINTS; i++)
    {
        float value = offset + VALUE::AVE + amplitude * (sinf(i * 0.1F)) + NextNoise();

        LIMITATION(value, static_cast<float>(VALUE::MIN), static_cast<float>(VALUE::MAX));

        data[i] = static_cast<uint8>(value);
    }

    return true;
}
