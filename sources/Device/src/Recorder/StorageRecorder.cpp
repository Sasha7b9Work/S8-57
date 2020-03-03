#include "defines.h"
#include "log.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/ExtRAM.h"
#include "Recorder/StorageRecorder.h"
#include "Settings/Settings.h"
#include <cstring>


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
    if(min > max)
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


int Record::NumPoints() const
{
    return numPoints;
}


void Record::AddPoints(BitSet16 dataA, BitSet16 dataB)
{
    HAL_BUS_CONFIGURE_TO_FSMC;

    LOG_WRITE("Добавяю точки %d %d , %d %d", dataA.byte0, dataA.byte1, dataB.byte0, dataB.byte1);

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


void Record::DeleteOldPoints()
{
    if(maxPoints == 0 || numPoints < maxPoints)
    {
        return;
    }

    uint numBytes = bytesOnPoint * static_cast<uint>(numPoints - 1);      // Столько байт будем перемещать

    uint8 *dest = BeginData();

    uint8 *src = dest + bytesOnPoint;

    std::memmove(dest, src, numBytes);
}


void Record::AddPoint(float value)
{
    HAL_BUS_CONFIGURE_TO_FSMC;

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

    float dMIN = (point2->min - point1->min) / (num2 - num1);
    float dMAX = (point2->max - point2->min) / (num2 - num1);

    for (int i = num1 + 1; i < num2; i++)
    {
        PointFloat *prev = ValueSensor(i - 1);
        PointFloat *point = ValueSensor(i);
        point->max = prev->max + dMAX;
        point->min = prev->min + dMIN;
    }
}


BitSet16 *Record::ValueA(int number)
{
    return reinterpret_cast<BitSet16 *>(AddressPoints(number));
}


BitSet16 *Record::ValueB(int number)
{
    return reinterpret_cast<BitSet16 *>(AddressPoints(number) + offsetB);
}


PointFloat *Record::ValueSensor(int number)
{
    return reinterpret_cast<PointFloat *>(AddressPoints(number) + offsetSensor);
}


uint8 *Record::BeginData()
{
    return reinterpret_cast<uint8 *>(this) + sizeof(Record);
}


uint8 *Record::AddressPoints(int number)
{
    return BeginData() + bytesOnPoint * number;
}


void Record::Init()
{
    maxPoints = 0;
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
