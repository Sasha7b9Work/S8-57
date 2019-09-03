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
    Lval_DIVIDER_A(this) = (uint)SET_DIVIDER(Chan::A);
    Lval_DIVIDER_B(this) = (uint)SET_DIVIDER(Chan::B);
    TIME_MS_DS(this) = 0;                        // Ёто важно дл€ режима поточеного вывода. ќзначает, что полный сигнал ещЄ не считан
    ENUM_POINTS(this) = FPGA_ENUM_POINTS;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DataSettings::Equals(const DataSettings &ds) const
{
    return
        RANGE_A(this) == Lval_RANGE_A(&ds) &&
        RANGE_B(this) == Lval_RANGE_B(&ds) &&
        RSHIFT_A(this) == RSHIFT_A(&ds) &&
        RSHIFT_B(this) == RSHIFT_B(&ds) &&
        TBASE(this) == TBASE(&ds) &&
        TSHIFT(this) == TSHIFT(&ds) &&
        TRIGLEV_A(this) == TRIGLEV_A(&ds) &&
        TRIGLEV_B(this) == TRIGLEV_B(&ds);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int DataSettings::SizeChannel() const
{
    Memory::ENumPointsFPGA points((Memory::ENumPointsFPGA::E)ENUM_POINTS(this));
    return (int)points.BytesInChannel(PEAKDET(this));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int DataSettings::PointsInChannel() const
{
    return (int)Memory::ENumPointsFPGA((Memory::ENumPointsFPGA::E)ENUM_POINTS(this)).PointsInChannel();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int DataSettings::NeedMemoryForData() const
{
    int result = 0;

    if (ENABLED_A(this))
    {
        result += SizeChannel();
    }

    if (ENABLED_B(this))
    {
        result += SizeChannel();
    }

    return result;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CHANGE(field, delta, max)                               \
if(delta > 0) { field = (field < (max)) ? ++field : 0; }        \
else          { field = (field > 0)         ? --field : (max); }

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PackedTime::ChangeHours(int delta)
{
    CHANGE(hours, delta, 23);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PackedTime::ChangeMinutes(int delta)
{
    CHANGE(minutes, delta, 59);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PackedTime::ChangeSeconds(int delta)
{
    CHANGE(seconds, delta, 59);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PackedTime::ChangeDay(int delta)
{
    CHANGE(day, delta, 31);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PackedTime::ChangeMonth(int delta)
{
    CHANGE(month, delta, 12);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PackedTime::ChangeYear(int delta)
{
    CHANGE(year, delta, 50);
}
