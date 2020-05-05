#include "defines.h"
#include "Hardware/Memory/IntRAM.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"
#include <cstring>


void DataSettings::Fill()
{
    Lval_ENABLED_A(this) = set.ch[ChanA].enabled ? 1U : 0U;
    Lval_ENABLED_B(this) = set.ch[ChanB].enabled ? 1U : 0U;

    INVERSE_A(this)      = set.ch[ChanA].inverse ? 1U : 0U;
    INVERSE_B(this)      = set.ch[ChanB].inverse ? 1U : 0U;

    Lval_RANGE_A(this)   = set.ch[ChanA].range;
    Lval_RANGE_B(this)   = set.ch[ChanB].range;

    RSHIFT_A(this)       = set.ch[ChanA].rShift;
    RSHIFT_B(this)       = set.ch[ChanB].rShift;

    Lval_TBASE(this)     = set.time.base;
    TSHIFT(this)         = set.time.shift;

    Lval_COUPLE_A(this)  = set.ch[ChanA].couple;
    Lval_COUPLE_B(this)  = set.ch[ChanB].couple;

    TRIGLEV_A(this)      = set.trig.level[ChanA];
    TRIGLEV_B(this)      = set.trig.level[ChanB];

    Lval_PEAKDET(this)   = PeakDetMode().IsEnabled() ? PeakDetMode::Enabled : PeakDetMode::Disabled;

    Lval_DIVIDER_A(this) = setA.divider;
    Lval_DIVIDER_B(this) = setB.divider;

    TIME_TIME(this)      = HAL_RTC::GetPackedTime();
    ENUM_POINTS(this)    = ENumPointsFPGA();
}


bool DataSettings::IsEquals(const DataSettings &ds) const
{
    return
        RANGE_A(this) == Lval_RANGE_A(&ds) &&
        RANGE_B(this) == Lval_RANGE_B(&ds) &&
        RSHIFT_A(this) == RSHIFT_A(&ds) &&
        RSHIFT_B(this) == RSHIFT_B(&ds) &&
        TBASE(this) == TBASE(&ds) &&
        TSHIFT(this) == TSHIFT(&ds) &&
        TRIGLEV_A(this) == TRIGLEV_A(&ds) &&
        TRIGLEV_B(this) == TRIGLEV_B(&ds) &&
        ENUM_POINTS(this) == ENUM_POINTS(&ds) &&
        PEAKDET(this) == PEAKDET(&ds);
}


bool DataSettings::EqualsCurrentSettings() const
{
    DataSettings ds;
    ds.Fill();
    return (this->IsEquals(ds));
}


uint DataSettings::BytesInChannel() const
{
    uint result = ENUM_TO_REL_POINTS(ENUM_POINTS(this));

    if (PEAKDET_ENABLED(this))
    {
        result *= 2;
    }

    return result;
}


uint DataSettings::PointsInChannel() const
{
    return ENUM_TO_REL_POINTS(ENUM_POINTS(this));
}


uint DataSettings::NeedMemoryForData() const
{
    uint result = 0;

    if (enableA)
    {
        result += BytesInChannel();
    }

    if (enableB)
    {
        result += BytesInChannel();
    }

    return result;
}



#define CHANGE(field, delta, max)                               \
if(delta > 0) { field = (field < (max)) ? ++field : 0; }        \
else          { field = (field > 0)         ? --field : (max); }


void PackedTime::ChangeHours(int delta)
{
    CHANGE(hours, delta, 23);
}


void PackedTime::ChangeMinutes(int delta)
{
    CHANGE(minutes, delta, 59);
}


void PackedTime::ChangeSeconds(int delta)
{
    CHANGE(seconds, delta, 59);
}


void PackedTime::ChangeDay(int delta)
{
    CHANGE(day, delta, 31);
}


void PackedTime::ChangeMonth(int delta)
{
    CHANGE(month, delta, 12);
}


void PackedTime::ChangeYear(int delta)
{
    CHANGE(year, delta, 50);
}


void DataSettings::CopyDataFrom(const DataSettings *source)
{
    uint numBytes = Math::Min(BytesInChannel(), source->BytesInChannel());

    if((enableA != 0) && (source->enableA != 0))
    {
        std::memcpy(dataA, source->dataA, numBytes);
    }

    if((enableB != 0) && (source->enableB != 0))
    {
        std::memcpy(dataB, source->dataB, numBytes);
    }
}
