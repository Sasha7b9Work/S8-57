#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Recorder/Recorder_Settings.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Recorder::Settings::ScaleX::Load()
{

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
