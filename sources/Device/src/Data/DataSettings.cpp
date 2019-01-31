#include "defines.h"
#include "Data/DataSettings.h"
#include "Settings/Settings.h"


using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DataSettings::Fill(uint8 *_dataA, uint8 * _dataB)
{
    dataA = _dataA;
    dataB = _dataB;

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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint DataSettings::SizeChannel() const
{
    Memory::ENumPointsFPGA points((Memory::ENumPointsFPGA::E)ENUM_POINTS(this));
    return points.BytesInChannel(PEAKDET(this));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint DataSettings::PointsInChannel() const
{
    return Memory::ENumPointsFPGA((Memory::ENumPointsFPGA::E)ENUM_POINTS(this)).PointsInChannel();
}

