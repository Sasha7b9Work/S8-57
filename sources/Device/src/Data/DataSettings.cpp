#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Heap.h"
#include "Data/Reader.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include <cstring>


uint FrameP2P::numBytesP2P = 0;
uint FrameP2P::pointerP2P = 0;
int  FrameP2P::posSeparate = 0;
DataSettings *FrameP2P::ds = nullptr;



void DataSettings::Fill()
{
    Lval_ENABLED_A(this) = ChanA.IsEnabled() ? 1U : 0U;
    Lval_ENABLED_B(this) = ChanB.IsEnabled() ? 1U : 0U;
    INVERSE_A(this)      = ChanA.IsInversed() ? 1U : 0U;
    INVERSE_B(this)      = ChanB.IsInversed() ? 1U : 0U;
    Lval_RANGE_A(this)   = Range(Chan::A);
    Lval_RANGE_B(this)   = Range(Chan::B);
    RSHIFT_A(this)       = RShift(Chan::A);
    RSHIFT_B(this)       = RShift(Chan::B);
    Lval_TBASE(this)     = TBase();
    TSHIFT(this)         = TShift();
    Lval_COUPLE_A(this)  = ModeCouple(Chan::A);
    Lval_COUPLE_B(this)  = ModeCouple(Chan::B);
    TRIGLEV_A(this)      = TrigLevel(Chan::A).Value();
    TRIGLEV_B(this)      = TrigLevel(Chan::B).Value();
    Lval_PEAKDET(this)   = PeakDetMode();
    Lval_DIVIDER_A(this) = Divider(Chan::A);
    Lval_DIVIDER_B(this) = Divider(Chan::B);
    TIME_TIME(this)      = HAL_RTC::GetPackedTime();
    ENUM_POINTS(this)    = ENumPointsFPGA();
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


bool DataSettings::EqualsCurrentSettings() const
{
    DataSettings ds;
    ds.Fill();
    return (this->Equals(ds));
}


uint DataSettings::BytesInChannel() const
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


void FrameP2P::AddPoint(const BitSet16 &a, const BitSet16 &b)
{
    if(PeakDetMode::IsEnabled())
    {
        AddPeakDetPoint(a.halfWord, b.halfWord);
        numBytesP2P += 2;
    }
    else
    {
        AddNormalPoint(a.byte0, b.byte1);
        numBytesP2P += 1;
    }
}


bool FrameP2P::IsCorrect()
{
    return (ds != nullptr);
}


void FrameP2P::AddNormalPoint(uint8 a, uint8 b)
{
    if(ds->enableA)
    {
        AddNormalPoint(Chan::A, a);
    }
    if(ds->enableB)
    {
        AddNormalPoint(Chan::B, b);
    }

    pointerP2P++;

    if(pointerP2P == ds->BytesInChannel())
    {
        pointerP2P = 0;
    }
}


void FrameP2P::AddPeakDetPoint(uint16 a, uint16 b)
{
    if(ds->enableA)
    {
        AddPeakDetPoint(Chan::A, a);
    }
    if(ds->enableB)
    {
        AddPeakDetPoint(Chan::B, b);
    }

    pointerP2P += 2;
    if(pointerP2P == ds->BytesInChannel())
    {
        pointerP2P = 0;
    }
}


void FrameP2P::AddNormalPoint(Chan::E ch, uint8 point)
{
    uint8 *data = (ch == Chan::A) ? ds->dataA : ds->dataB;

    data[pointerP2P] = point;
}


void FrameP2P::AddPeakDetPoint(Chan::E ch, uint16 point)
{
    uint8 *data = (ch == Chan::A) ? ds->dataA : ds->dataB;

    data += pointerP2P;

    uint16 *d16 = reinterpret_cast<uint16 *>(data);

    *d16 = point;
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


void FrameP2P::FillScreenBuffer(Buffer *buffer, Chan::E ch)
{
    uint8 *out = buffer->data;
    std::memset(out, VALUE::NONE, buffer->Size());

    posSeparate = MAX_UINT;

    if(numBytesP2P > 0 && numBytesP2P <= buffer->Size())
    {
        std::memcpy(out, OUT(ch), numBytesP2P);
        posSeparate = static_cast<int>(numBytesP2P - 1);
    }
    else
    {
        uint size = GetNumberStoredBytes();
        if(size < ds->BytesInChannel())
        {
            size = ds->BytesInChannel();
        }

        uint pointer = 0;

        for(uint numByte = 0; numByte < size; numByte++)
        {
            uint8 d = GetByte(numByte, ch);

            if(d != VALUE::NONE)
            {
                out[pointer] = d;
            }

            if((d == VALUE::NONE) && (posSeparate == MAX_UINT))
            {
                posSeparate = static_cast<int>(pointer);
            }

            pointer++;
            if(pointer == buffer->Size())
            {
                pointer = 0;
            }
        }
    }
}


uint FrameP2P::GetNumberStoredBytes()
{
    if(numBytesP2P < ds->BytesInChannel())
    {
        return numBytesP2P;
    }

    return ds->BytesInChannel();
}


uint8 FrameP2P::GetByte(uint position, Chan::E ch)
{
    if(GetNumberStoredBytes() < ds->BytesInChannel())
    {
        uint8 *data = OUT(ch);

        return (position < numBytesP2P) ? data[position] : VALUE::NONE;
    }

    return VALUE::NONE;
}


void FrameP2P::Prepare()
{
    if(!Osci::InModeP2P())
    {
        ds = nullptr;

        return;
    }

    ds = RAM::PrepareForNewData();

    posSeparate = 0;

    numBytesP2P = 0;

    pointerP2P = 0;

    std::memset(ds->dataA, VALUE::NONE, ds->BytesInChannel());

    std::memset(ds->dataB, VALUE::NONE, ds->BytesInChannel());
}
