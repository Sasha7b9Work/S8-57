#include "defines.h"
#include "FPGA/FPGA_Math.h"
#include "Settings/Settings.h"
#include "Utils/Values.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Tester::Settings::Scale::ToString() const // -V2506
{
    if (Chan(ch).IsA())
    {
        return Osci::Settings::Range((Osci::Settings::Range::E)value).ToString(Divider::_1);
    }

    static const pString names[][Language::Size] =
    {
        "2ÏÍ¿",     "2uA",
        "5ÏÍ¿",     "5u¿",
        "10ÏÍ¿",    "10u¿",
        "20ÏÍ¿",    "20u¿",
        "50ÏÍ¿",    "50u¿",
        "100ÏÍ¿",   "100u¿",
        "200ÏÍ¿",   "200u¿",
        "500ÏÍ¿",   "500u¿",
        "1Ï¿",      "1m¿",
        "2Ï¿",      "2m¿",
        "5Ï¿",      "5m¿",
        "10Ï¿",     "10m¿",
        "20Ï¿",     "20m¿"
    };

    pString name = names[value][LANG];

    if (name)
    {
        return names[value][LANG];
    }
    else
    {
        LOG_ERROR("Ã‡ÒÒË‚ ÌÂ Á‡ÔÓÎÌÂÌ");
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Tester::Settings::Shift::ToString(Scale::E scale) // -V2506
{
    if (ch == Chan::A)
    {
        return Osci::Settings::RShift::ToString(shift, (Osci::Settings::Range::E)scale, Divider::_1);
    }

    float shiftAbs = FPGA::Math::RShift2Abs(shift, (Osci::Settings::Range::E)scale) * 1e-3F;

    return Current(shiftAbs).ToString();
}
