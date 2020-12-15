#include "defines.h"
#include "log.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/ExtRAM.h"
#include "Recorder/DisplayRecorder.h"
#include "Recorder/StorageRecorder.h"
#include "Settings/Settings.h"
#include <cstring>


BitSet16 BufferMissingPoints::points[2][256];

int BufferMissingPoints::last = 0;

int BufferMissingPoints::first = 0;


// Последняя запись. Если идёт запись, то именно в неё.
static Record *lastRecord = nullptr;

static PointFloat empty = { 1.0F, -1.0F };

#define EXIST_A      (sources & (1 << 0))
#define EXIST_B      (sources & (1 << 1))
#define EXIST_A_OR_B (sources & (1 << 3))
#define EXIST_SENS   (sources & (1 << 2))


void BufferMissingPoints::Push(BitSet16 a, BitSet16 b)
{
    points[0][last] = a;
    points[1][last] = b;
    last++;
}

void BufferMissingPoints::Pop(BitSet16 *a, BitSet16 *b)
{
    *a = points[0][first];
    *b = points[1][first];
    first++;

    if(last == first)
    {
        last = 0;
        first = 0;
    }
}


Point16 *Point16::Next(Record *record) const
{
    return const_cast<Point16 *>(reinterpret_cast<const Point16 *>(reinterpret_cast<const uint8 *>(this) + record->bytesOnPoint)); //-V2563 //-V2567
}


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
    if(min > max)
    {
        min = value;
        max = value;
    }
    else if(value < min)
    {
        min = value;
    }
    else if(value > max) //-V2516
    {
        max = value;
    }
}


int Record::NumPoints() const
{
    return numPoints;
}


void Record::AddPoints(BitSet16 dataA, BitSet16 dataB) //-V2506
{
    if(DisplayRecorder::InProcessUpdate())
    {
        BufferMissingPoints::Push(dataA, dataB);

        return;
    }

    HAL_BUS_CONFIGURE_TO_FSMC();

    if(maxPoints)
    {
        DeleteOldPoints();
    }

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


void Record::AddMissingPoints()
{
    while(BufferMissingPoints::Size())
    {
        BitSet16 a;
        BitSet16 b;

        BufferMissingPoints::Pop(&a, &b);
         
        AddPoints(a, b);
    }
}


void Record::DeleteOldPoints()
{
    if(numPoints == maxPoints)
    {
        uint numBytes = bytesOnPoint * static_cast<uint>(numPoints - 1);      // Столько байт будем перемещать

        uint8 *dest = BeginData();

        uint8 *src = dest + bytesOnPoint; //-V2563

        std::memmove(dest, src, numBytes);

        numPoints--;
    }
}


void Record::AddPoint(float value)
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    if(EXIST_SENS)
    {
        ValueSensor(numPoints)->Add(value);

        // Теперь интерполируем отсутствующие точки

        int i = numPoints - 1;

        for(; i >= 0; i--)
        {
            if (!ValueSensor(i)->IsEmpty())
            {
                break;
            }
        }

        if (numPoints - i > 1)
        {
            Interpolate(i, numPoints);
        }
    }
}


void Record::Interpolate(int num1, int num2)
{
    PointFloat *point1 = ValueSensor(num1);
    PointFloat *point2 = ValueSensor(num2);

    float dMIN = (point2->min - point1->min) / (num2 - num1); //-V2564
    float dMAX = (point2->max - point2->min) / (num2 - num1); //-V2564

    for (int i = num1 + 1; i < num2; i++)
    {
        PointFloat *prev = ValueSensor(i - 1);
        PointFloat *point = ValueSensor(i);
        point->max = prev->max + dMAX;
        point->min = prev->min + dMIN;
    }
}


Point16 *Record::ValueA(int number)
{
    return reinterpret_cast<Point16 *>(AddressPoints(number));
}


Point16 *Record::ValueB(int number)
{
    return reinterpret_cast<Point16 *>(AddressPoints(number) + offsetB); //-V2563
}


PointFloat *Record::ValueSensor(int number)
{
    return EXIST_SENS ? reinterpret_cast<PointFloat *>(AddressPoints(number) + offsetSensor) : &empty; //-V2563
}


uint8 *Record::BeginData()
{
    return reinterpret_cast<uint8 *>(this) + sizeof(Record); //-V2563
}


uint8 *Record::AddressPoints(int number)
{
    return BeginData() + bytesOnPoint * number; //-V2563
}


void Record::Init()
{
    maxPoints = 0;
    timeStart = HAL_RTC::GetPackedTime();
    numPoints = 0;
    sources = 0;
    bytesOnPoint = 0;
    timeForPointMS = static_cast<uint>(Recorder::ScaleX::TimeForPointMS());

    offsetB = 0;
    offsetSensor = 0;

    if(S_REC_ENABLED_A)
    {
        sources |= (1 << 0);
        bytesOnPoint += 2;

        offsetB = sizeof(BitSet16);
        offsetSensor = sizeof(BitSet16);
    }

    if(S_REC_ENABLED_B)
    {
        sources |= (1 << 1);
        bytesOnPoint += 2;

        offsetSensor += sizeof(BitSet16);
    }

    if(S_REC_ENABLED_SENSOR)
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
    HAL_BUS_CONFIGURE_TO_FSMC();

    return Begin() + sizeof(*this) + bytesOnPoint * numPoints; //-V2563
}


bool Record::IsValid() const //-V2506
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    if(Begin() < ExtRAM::Begin() || (End() + 1024) > ExtRAM::End()) //-V2563
    {
        return false;
    }

    return true;
}


bool Record::ContainsChannelA() const
{
    return (sources & 0x01) != 0;
}


bool Record::ContainsChannelB() const
{
    return (sources & 0x02) != 0;
}


bool Record::ContainsSensor() const
{
    return (sources & 0x04) != 0;
}


Record *StorageRecorder::LastRecord()
{
    return lastRecord;
}


bool StorageRecorder::CreateNewRecord() //-V2506
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    if(lastRecord)
    {
        Record *next = reinterpret_cast<Record *>(lastRecord ->End());

        if(!next->IsValid())
        {
            return false;
        }

        lastRecord = next;
    }
    else
    {
        lastRecord = reinterpret_cast<Record *>(ExtRAM::Begin());
    }

    lastRecord->Init();

    return true;
}


bool StorageRecorder::CreateListeningRecord()
{
    bool result = CreateNewRecord();

    if (result)
    {
        LastRecord()->SetMaxPoints(320);
    }

    return result;
}


void StorageRecorder::Init()
{
    ExtRAM::Fill();
    lastRecord = nullptr;
}


uint StorageRecorder::NumRecords()
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    const Record *record = reinterpret_cast<Record *>(ExtRAM::Begin());

    uint result = 0;

    while(record->IsValid())
    {
        record = reinterpret_cast<const Record *>(record->End());
        result++;
    }

    return result;
}
