#include "defines.h"
#include "Data/DataSettings.h"
#include "Settings/Settings.h"


void DataSettings::Fill(uint8 *_dataA, uint8 * _dataB)
{
    dataA = _dataA;
    dataB = _dataB;

    Lval_ENABLED_A(this) = set.ch[Chan::A].enabled ? 1U : 0U;
    Lval_ENABLED_B(this) = set.ch[Chan::B].enabled ? 1U : 0U;
    INVERSE_A(this) = set.ch[Chan::A].inverse ? 1U : 0U;
    INVERSE_B(this) = set.ch[Chan::B].inverse ? 1U : 0U;
    Lval_RANGE_A(this) = Range(Chan::A);
    Lval_RANGE_B(this) = Range(Chan::B);
    RSHIFT_A(this) = RShift(Chan::A);
    RSHIFT_B(this) = RShift(Chan::B);
    Lval_TBASE(this) = static_cast<uint>(TBase());
    TSHIFT(this) = TShift();
    Lval_COUPLE_A(this) = static_cast<uint>(ModeCouple(Chan::A));
    Lval_COUPLE_B(this) = static_cast<uint>(ModeCouple(Chan::B));
    TRIGLEV_A(this) = set.trig.lev[Chan::A];
    TRIGLEV_B(this) = set.trig.lev[Chan::B];
    Lval_PEAKDET(this) = static_cast<uint>(PeakDetMode());
    Lval_DIVIDER_A(this) = static_cast<uint>(set.ch[Chan::A].divider);
    Lval_DIVIDER_B(this) = static_cast<uint>(set.ch[Chan::B].divider);
    TIME_MS_DS(this) = 0;                        // Ёто важно дл€ режима поточеного вывода. ќзначает, что полный сигнал ещЄ не считан
    ENUM_POINTS(this) = static_cast<uint>(ENumPointsFPGA());
}


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
        TRIGLEV_B(this) == TRIGLEV_B(&ds) &&
        ENUM_POINTS(this) == ENUM_POINTS(&ds);
}


uint DataSettings::SizeChannel() const
{
    uint result = ENUM_TO_REL_POINTS(ENUM_POINTS(this));

    if (PEAKDET(this) == PeakDetMode::Enabled)
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
        result += SizeChannel();
    }

    if (enableB)
    {
        result += SizeChannel();
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


void FrameP2P::AddPoints(BitSet16, BitSet16)
{

}


void FrameP2P::FillBufferForDraw(Chan::E, Buffer *)
{

}


void DataSettings::Log() const
{
    if (enableA)
    {
        uint8 *data = dataA;
        LOG_WRITE("A: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
            data[ 0], data[ 1], data[ 2], data[ 3], data[ 4], data[ 5], data[ 6], data[ 7], data[ 8], data[ 9],
            data[10], data[11], data[12], data[13], data[14], data[15], data[16], data[17], data[18], data[19]);
    }
    else
    {
        LOG_WRITE("A: откл");
    }

    if (enableB)
    {
        uint8 *data = dataB;
        LOG_WRITE("B: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
            data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9],
            data[10], data[11], data[12], data[13], data[14], data[15], data[16], data[17], data[18], data[19]);
    }
    else
    {
        LOG_WRITE("B: откл");
    }
}
