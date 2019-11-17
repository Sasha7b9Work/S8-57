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

    const float ampl = 2.0F;    // Амплитуда шума
    const float step = 7.0F;

    float min = prev - step;
    LIMIT_BELOW(min, -ampl);

    float max = prev + step;
    LIMIT_ABOVE(max, ampl);

    prev = Math::RandFloat(min, max);

    return prev;
}


static bool GenerateNormalModeData(Chan::E ch, uint8 data[ENumPointsFPGA::MAX_NUM])
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

    double amplitude = TuneGeneratorDialog::amplitude[ch] / MathFPGA::RShift2Abs(1, set.ch[ch].range) * 315;

    double frequency = TuneGeneratorDialog::frequency[ch] * MathFPGA::TShift2Abs(1, set.time.base);

    double offset = MathFPGA::RShift2Abs(set.ch[ch].rShift, set.ch[ch].range) * kOffset[set.ch[ch].range];

    for (uint i = 0; i < ENumPointsFPGA::MAX_NUM; i++)
    {
        double value = offset + VALUE::AVE + amplitude * (sin(2 * Math::PI * i * frequency)) + NextNoise();

        LIMITATION(value, static_cast<float>(VALUE::MIN), static_cast<float>(VALUE::MAX));

        data[i] = static_cast<uint8>(value);
    }


    return true;
}


bool FPGA::ReadDataChanenl(Chan::E ch, uint8 data[ENumPointsFPGA::MAX_NUM])
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

    double amplitude = 100.0 * TuneGeneratorDialog::amplitude[ch];
    double offset = 0.0 + TuneGeneratorDialog::offset[ch];

    for (uint i = 0; i < FPGA_NUM_POINTS; i++)
    {
        double value = offset + VALUE::AVE + amplitude * (sin(i * 0.1)) + NextNoise();

        LIMITATION(value, static_cast<float>(VALUE::MIN), static_cast<float>(VALUE::MAX));

        data[i] = static_cast<uint8>(value);
    }

    return true;
}
