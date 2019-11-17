#include "defines.h"
#include "FPGA/FPGA.h"
#include "GUI/Dialogs/TuneGeneratorDialog.h"
#include "Osci/Osci.h"
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


static bool GenerateNormalModeData(Chan::E ch, uint8 data[FPGA::MAX_NUM_POINTS])
{
    static const float kOffset[Range::Count] =
    {
        12500.0F,  //   2 mV
        5000.0F,   //   5 mV
        2500.0F,   //  10 mV
        1250.0F,   //  20 mV
        500.0F,    //  50 mV
        250.0F,    // 100 mV
        125.0F,    // 200 mV
        50.0F,     // 500 mV
        25.0F,     //   1 V
        12.5F,     //   2 V
        5.0F,      //   5 V
        2.5F,      //  10 V
        1.25F      //  20 V
    };

    float amplitude = TuneGeneratorDialog::amplitude / MathFPGA::RShift2Abs(1, set.ch[ch].range) * 315;

    float frequency = TuneGeneratorDialog::frequency * MathFPGA::TShift2Abs(1, set.time.base);

    float offset = MathFPGA::RShift2Abs(set.ch[ch].rShift, set.ch[ch].range) * kOffset[set.ch[ch].range];

    for (uint i = 0; i < FPGA::MAX_NUM_POINTS; i++)
    {
        float value = offset + VALUE::AVE + amplitude * (sinf(2 * Math::PI_F * i * frequency));

        LIMITATION(value, static_cast<float>(VALUE::MIN), static_cast<float>(VALUE::MAX));

        data[i] = static_cast<uint8>(value);
    }


    return true;
}


bool FPGA::ReadDataChanenl(Chan::E ch, uint8 data[MAX_NUM_POINTS])
{
    if (!set.ch[ch].enabled)
    {
        return false;
    }

    if (Osci::InModeP2P())
    {

    }
    else if (Osci::InModeRandomizer())
    {

    }
    else
    {
        return GenerateNormalModeData(ch, data);
    }

    float amplitude = 100.0F * TuneGeneratorDialog::amplitude;
    float offset = 0.0F + TuneGeneratorDialog::offset;

    for (uint i = 0; i < FPGA_NUM_POINTS; i++)
    {
        float value = offset + VALUE::AVE + amplitude * (sinf(i * 0.1F)) + NextNoise();

        LIMITATION(value, static_cast<float>(VALUE::MIN), static_cast<float>(VALUE::MAX));

        data[i] = static_cast<uint8>(value);
    }

    return true;
}
