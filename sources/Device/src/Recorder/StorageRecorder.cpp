#include "defines.h"
#include "log.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/ExtRAM.h"
#include "Recorder/StorageRecorder.h"
#include "Settings/Settings.h"


// Последняя запись. Если идёт запись, то именно в неё.
static Record *last = nullptr;


#define EXIST_A      (sources & (1 << 0))
#define EXIST_B      (sources & (1 << 1))
#define EXIST_A_OR_B (sources & (1 << 3))
#define EXIST_SENS   (sources & (1 << 2))


void PointFloat::Prepare()
{
    min = 1.0F;
    max = -1.0F;
}


bool PointFloat::IsEmpty() const
{
    return (min > max);
}


void PointFloat::Add(float value)
{
    if(IsEmpty())
    {
        min = value;
        max = value;
    }
    else if(value < min)
    {
        min = value;
    }
    else if(value > max)
    {
        max = value;
    }
}


uint Record::NumPoints() const
{
    return numPoints;
}


void Record::AddPoints(BitSet16 dataA, BitSet16 dataB)
{
    HAL_BUS_CONFIGURE_TO_FSMC;

    if(EXIST_A)
    {
        *ValueA(numPoints) = dataA;
    }

    if(EXIST_B)
    {
        *ValueB(numPoints) = dataB;
    }

    numPoints++;

    ValueSensor(numPoints)->Prepare();
}


void Record::AddPoint(float value)
{
    HAL_BUS_CONFIGURE_TO_FSMC;

    if(EXIST_SENS)
    {
        ValueSensor(numPoints)->Add(value);

        // Теперь интерполируем отсутствующие точки

        for(uint i = numPoints; i < 0xFFFFFFFF; i--)
        {

        }
    }
}


BitSet16 *Record::ValueA(uint number)
{
    return reinterpret_cast<BitSet16 *>(AddressPoints(number));
}


BitSet16 *Record::ValueB(uint number)
{
    return reinterpret_cast<BitSet16 *>(AddressPoints(number) + offsetB);
}


PointFloat *Record::ValueSensor(uint number)
{
    return reinterpret_cast<PointFloat *>(AddressPoints(number) + offsetSensor);
}


uint8 *Record::BeginData()
{
    return reinterpret_cast<uint8 *>(this) + sizeof(Record);
}


uint8 *Record::AddressPoints(uint number)
{
    return BeginData() + bytesOnPoint * number;
}


void Record::Init()
{
    timeStart = HAL_RTC::GetPackedTime();
    numPoints = 0;
    sources = 0;
    bytesOnPoint = 0;
    offsetB = 0;
    offsetSensor = 0;

    if(set.rec.enA)
    {
        sources |= (1 << 0);
        bytesOnPoint += 2;

        offsetB = sizeof(BitSet16);
        offsetSensor = sizeof(BitSet16);
    }

    if(set.rec.enB)
    {
        sources |= (1 << 1);
        bytesOnPoint += 2;

        offsetSensor += sizeof(BitSet16);
    }

    if(set.rec.enSensor)
    {
        sources |= (1 << 2);
        bytesOnPoint += sizeof(float) * 2;      // Каждая точка датчика требует 2 значения типа float - минимальное и максимальное
    }

    ValueSensor(0)->Prepare();
}


uint Record::FreeMemory() const
{
    return 5;
}


uint8 *Record::Begin() const
{
    return reinterpret_cast<uint8 *>(const_cast<Record *>(this));
}


uint8 *Record::End() const
{
    HAL_BUS_CONFIGURE_TO_FSMC;

    return Begin() + sizeof(*this) + bytesOnPoint * numPoints;
}


bool Record::IsValid() const
{
    HAL_BUS_CONFIGURE_TO_FSMC;

    if(Begin() < ExtRAM::Begin() || (End() + 1024) > ExtRAM::End())
    {
        return false;
    }

    return true;
}


Record *StorageRecorder::LastRecord()
{
    return last;
}


bool StorageRecorder::CreateNewRecord()
{
    HAL_BUS_CONFIGURE_TO_FSMC;

    if(last)
    {
        Record *next = reinterpret_cast<Record *>(last->End());

        if(!next->IsValid())
        {
            return false;
        }

        last = next;
    }
    else
    {
        last = reinterpret_cast<Record *>(ExtRAM::Begin());
    }

    last->Init();

    return true;
}


void StorageRecorder::Init()
{
    ExtRAM::Fill();
    last = nullptr;
}


uint StorageRecorder::NumRecords()
{
    HAL_BUS_CONFIGURE_TO_FSMC;

    const Record *record = reinterpret_cast<Record *>(ExtRAM::Begin());

    uint result = 0;

    while(record->IsValid())
    {
        record = reinterpret_cast<const Record *>(record->End());
        result++;
    }

    return result;
}
