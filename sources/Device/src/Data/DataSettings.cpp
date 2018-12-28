#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "DataSettings.h"
#include "Utils/Measure.h"
#include "Settings/Settings.h"
#include <cstring>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint DataSettings::lastID = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DataSettings::Erase()
{
    std::memset(this, 0, sizeof(DataSettings));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int DataSettings::SizeChannel() const
{
    static const int numPoints[ENumPointsFPGA::Size][3] =
    {
        {512,   1024,  1024},
        {1024,  2048,  2048},
        {2048,  4096,  4096},
        {4096,  8192,  8192},
        {8192,  16384, 16384}
    };

    return numPoints[ENUM_POINTS(this)][PEAKDET(this)];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int DataSettings::SizeData() const
{
    int numChannels = 0;
    if(enableA) { ++numChannels; }
    if(enableB) { ++numChannels; }

    return SizeChannel() * numChannels;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataSettings::Fill(uint8 *datA, uint8 *datB)
{
    dataA = datA;
    dataB = datB;

    Lval_ENABLED_A(this) = SET_ENABLED_A ? 1U : 0U;
    Lval_ENABLED_B(this) = SET_ENABLED_B ? 1U : 0U;
    INVERSE_A(this) = SET_INVERSE_A ? 1U : 0U;
    INVERSE_B(this) = SET_INVERSE_B ? 1U : 0U;
    Lval_RANGE_A(this) = SET_RANGE_A;
    Lval_RANGE_B(this) = SET_RANGE_B;
    RSHIFT_A(this) = SET_RSHIFT_A;
    RSHIFT_B(this) = SET_RSHIFT_B;
    Lval_TBASE(this) = SET_TBASE;
    TSHIFT(this) = (int16)SET_TSHIFT;
    Lval_COUPLE_A(this) = SET_COUPLE_A;
    Lval_COUPLE_B(this) = SET_COUPLE_B;
    TRIGLEV_A(this) = SET_TRIGLEV_A;
    TRIGLEV_B(this) = SET_TRIGLEV_A;
    Lval_PEAKDET(this) = SET_PEAKDET;
    Lval_DIVIDER_A(this) = SET_DIVIDER_A;
    Lval_DIVIDER_B(this) = SET_DIVIDER_B;
    TIME_MS_DS(this) = 0;                        // Ёто важно дл€ режима поточеного вывода. ќзначает, что полный сигнал ещЄ не считан
    ENUM_POINTS(this) = FPGA_ENUM_POINTS;
}
