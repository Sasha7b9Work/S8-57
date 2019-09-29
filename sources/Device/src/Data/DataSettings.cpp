#include "defines.h"
#include "Data/DataSettings.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DataSettings::Fill(uint8 *_dataA, uint8 * _dataB)
{
    dataA = _dataA;
    dataB = _dataB;

    Lval_ENABLED_A(this) = set.ch[Chan::A].enabled ? 1U : 0U;
    Lval_ENABLED_B(this) = set.ch[Chan::B].enabled ? 1U : 0U;
    INVERSE_A(this) = set.ch[Chan::A].inverse ? 1U : 0U;
    INVERSE_B(this) = set.ch[Chan::B].inverse ? 1U : 0U;
    Lval_RANGE_A(this) = set.ch[Chan::A].range;
    Lval_RANGE_B(this) = set.ch[Chan::B].range;
    RSHIFT_A(this) = SET_RSHIFT_A;
    RSHIFT_B(this) = SET_RSHIFT_B;
    Lval_TBASE(this) = set.time.base;
    TSHIFT(this) = (int16)set.time.shift;
    Lval_COUPLE_A(this) = set.ch[Chan::A].couple;
    Lval_COUPLE_B(this) = set.ch[Chan::B].couple;
    TRIGLEV_A(this) = set.trig.lev[Chan::A];
    TRIGLEV_B(this) = set.trig.lev[Chan::B];
    Lval_PEAKDET(this) = set.time.peakDet;
    Lval_DIVIDER_A(this) = (uint)set.ch[Chan::A].divider;
    Lval_DIVIDER_B(this) = (uint)set.ch[Chan::B].divider;
    TIME_MS_DS(this) = 0;                        // Ёто важно дл€ режима поточеного вывода. ќзначает, что полный сигнал ещЄ не считан
    ENUM_POINTS(this) = set.mem.enumPoints;
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
    ENumPointsFPGA points((ENumPointsFPGA::E)ENUM_POINTS(this));
    return (int)points.BytesInChannel(PEAKDET(this));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int DataSettings::PointsInChannel() const
{
    return (int)ENumPointsFPGA((ENumPointsFPGA::E)ENUM_POINTS(this)).PointsInChannel();
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
