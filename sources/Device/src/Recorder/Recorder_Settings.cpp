#include "defines.h"
#include "Recorder/Recorder_Settings.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"

#include "Recorder/Recorder.h"
#include "Hardware/HAL/HAL.h"
#include "FPGA/FPGA.h"


#ifdef WIN32
#pragma warning(disable:4310)
#endif


using namespace FPGA::ADDR;

using HAL::FSMC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Recorder::Settings::ScaleX::Load()
{
    static const uint8 values[ScaleX::Size] =
    {
        BIN_U8(01010110),  // -V2501  // 5 мс   100ms  
        BIN_U8(01010111),  // -V2501  // 10 мс  200ms  
        BIN_U8(01011001),  // -V2501  // 25 мс  500ms  
        BIN_U8(01011010),  // -V2501  // 50 мс  1s     
        BIN_U8(01011011),  // -V2501  // 100 мс 2s     
        BIN_U8(01011101),  // -V2501  // 200 мс 5s     
        BIN_U8(01011110),  // -V2501  // 500 мс 10s
        BIN_U8(01011110),  // -V2501  // 1 с    10s
        BIN_U8(01011110),  // -V2501  // 2 с    10s
        BIN_U8(01011110),  // -V2501  // 5 с    10s
        BIN_U8(01011110),  // -V2501  // 10 с   10s
        BIN_U8(01011110),  // -V2501  // 20 с   10s
        BIN_U8(01011110),  // -V2501  // 30 с   10s
        BIN_U8(01011110)   // -V2501  // 60 с   10s
    };

    FSMC::WriteToFPGA8(WR::TBASE, values[RECORDER_SCALE_X.value]);

    if (Recorder::IsRunning())
    {
        Stop();
        Start();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Settings::ScaleX::Change(int delta)
{
    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>((uint8 *)(&set.rec_scaleX), (uint8)(ScaleX::Size - 1));
    }
    else
    {
        ::Math::LimitationDecrease<uint8>((uint8 *)(&set.rec_scaleX), 0);
    }

    Load();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Recorder::Settings::ScaleX &Recorder::Settings::ScaleX::Current()
{
    return set.rec_scaleX;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Recorder::Settings::ScaleX::ToString() const
{
    static const struct StructScaleX
    {
        const char *name[Language::Size];
        StructScaleX(pString nRU, pString nEN)
        {
            name[Language::RU] = nRU;
            name[Language::EN] = nEN;
        };
    }
    scales[ScaleX::Size] =
    {
        StructScaleX("5\x10мс",   "5\x10ms"),
        StructScaleX("10\x10мс",  "10\x10ms"),
        StructScaleX("25\x10мс",  "25\x10ms"),
        StructScaleX("50\x10мс",  "50\x10ms"),
        StructScaleX("100\x10мс", "100\x10ms"),
        StructScaleX("250\x10мс", "250\x10ms"),
        StructScaleX("500\x10мс", "500\x10ms"),
        StructScaleX("1\x10с",    "1\x10s"),
        StructScaleX("2\x10с",    "2\x10s"),
        StructScaleX("5\x10с",    "5\x10s"),
        StructScaleX("10\x10с",   "10\x10s"),
        StructScaleX("20\x10с",   "20\x10s"),
        StructScaleX("30\x10с",   "30\x10s"),
        StructScaleX("60\x10с",   "60\x10s"),
    };

    return scales[value].name[LANG];
}
