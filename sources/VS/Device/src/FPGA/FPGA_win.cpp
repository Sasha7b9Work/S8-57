#include "defines.h"
#include "FPGA/FPGA.h"
#include "GUI/Dialogs/TuneGeneratorDialog.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <cstdlib>
#include <cstring>


uint16 FPGA::addrRead = 0;


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


static bool GenerateNormalModeData(Chan::E ch, uint8 *data, uint numBytes)
{
    static const double kOffset[Range::Count] =
    {
        12500.0,    //   2 mV
        5000.0,     //   5 mV
        2500.0,     //  10 mV
        1250.0,     //  20 mV
        500.0,      //  50 mV
        250.0,      // 100 mV
        125.0,      // 200 mV
        50.0,       // 500 mV
        25.0,       //   1 V
        12.5,       //   2 V
        5.0,        //   5 V
        2.5,        //  10 V
        1.25        //  20 V
    };

    double amplitude = TuneGeneratorDialog::amplitude[ch] / RShift::ToAbs(1, Range(ch)) * 0.6;

    double frequency = TuneGeneratorDialog::frequency[ch] * TShift::ToAbs(1, TBase());

    double offset = RShift::ToAbs(RShift(ch), Range(ch)) * kOffset[Range(ch)];

    if(ModeCouple(ch) == ModeCouple::DC)
    {
        offset += TuneGeneratorDialog::offset[ch] * kOffset[Range(ch)];
    }

    for (uint i = 0; i < numBytes; i++)
    {
        double value = offset + VALUE::AVE + amplitude * (sin(2 * Math::PI * i * frequency)) + NextNoise();

        LIMITATION(value, static_cast<float>(VALUE::MIN), static_cast<float>(VALUE::MAX));

        data[i] = static_cast<uint8>(value);
    }

    return true;
}


bool FPGA::ReadDataChanenl(Chan::E ch, uint8 *data, uint numBytes)
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
        return GenerateNormalModeData(ch, data, numBytes);
    }

    double amplitude = 100.0 * TuneGeneratorDialog::amplitude[ch];
    double offset = 0.0 + TuneGeneratorDialog::offset[ch];

    for (uint i = 0; i < numBytes; i++)
    {
        double value = offset + VALUE::AVE + amplitude * (sin(i * 0.1)) + NextNoise();

        LIMITATION(value, static_cast<float>(VALUE::MIN), static_cast<float>(VALUE::MAX));

        data[i] = static_cast<uint8>(value);
    }

    return true;
}
