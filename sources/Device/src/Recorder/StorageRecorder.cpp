// 2023/11/22 13:45:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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

#define EXIST_A      (sources & (1 << 0))
#define EXIST_B      (sources & (1 << 1))
#define EXIST_A_OR_B (sources & (1 << 3))


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
    return const_cast<Point16 *>(reinterpret_cast<const Point16 *>(reinterpret_cast<const uint8 *>(this) + record->bytesOnPoint)); //-V2567
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


void Record::AddPoints(BitSet16 dataA, BitSet16 dataB)
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

        uint8 *src = dest + bytesOnPoint;

        std::memmove(dest, src, numBytes);

        numPoints--;
    }
}


Point16 *Record::ValueA(int number)
{
    return reinterpret_cast<Point16 *>(AddressPoints(number));
}


Point16 *Record::ValueB(int number)
{
    return reinterpret_cast<Point16 *>(AddressPoints(number) + offsetB);
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
    timeForPointMS = static_cast<uint>(Recorder::ScaleX::TimeForPointMS());
    offsetB = 0;

    if(S_REC_ENABLED_A)
    {
        sources |= (1 << 0);
        bytesOnPoint += 2;
        offsetB = sizeof(BitSet16);
    }

    if(S_REC_ENABLED_B)
    {
        sources |= (1 << 1);
        bytesOnPoint += 2;
    }
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

    return Begin() + sizeof(*this) + bytesOnPoint * numPoints;
}


bool Record::IsValid() const
{
    HAL_BUS_CONFIGURE_TO_FSMC();

    if(Begin() < ExtRAM::Begin() || (End() + 1024) > ExtRAM::End())
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


Record *StorageRecorder::LastRecord()
{
    return lastRecord;
}


bool StorageRecorder::CreateNewRecord(char * /*file*/, int /*line*/)
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
    bool result = CreateNewRecord(__FILE__, __LINE__);

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
