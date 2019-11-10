#include "defines.h"
#include "Data/Heap.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/MemoryROM.h"
#include "Recorder/StorageRecorder.h"



/// Обёртка для Heap()
struct Stack
{
    static Record record;

    static void Push(const Record &_record)
    {
        record = _record;
        record.SetDataAddress((uint16 *)SECTOR(Sector::_17_RECORDER_1).address);
    }

    static Record *Top()
    {
        return &record;
    }
};

Record Stack::record;


uint Record::NumPoints()
{
    return numPoints;
}


int Record::FreeMemory()
{
    return 1024 * 128 - static_cast<int>(sizeof(Point) * numPoints);
}


void Record::AddPoint(BitSet16 dataA, BitSet16 dataB)
{
    BitSet32 bs(dataA.halfWord, dataB.halfWord);

    FlashMemory::Write(SECTOR(Sector::_17_RECORDER_1).address + numPoints * sizeof(Point), &bs, sizeof(BitSet32));

    numPoints++;
}


Record *StorageRecorder::CurrentRecord()
{
    return Stack::Top();
}


void StorageRecorder::CreateNewRecord()
{
    Record record;
    Stack::Push(record);
}


void Record::SetDataAddress(uint16 *address)
{
    start = reinterpret_cast<Point *>(address);
    numPoints = 0;
    pointer = MAX_UINT;
}


Point Record::GetPoint(uint position, uint maxPoints)
{
    pointer = position - 1;

    return NextPoint(maxPoints);
}


Point Record::NextPoint(uint maxPoints)
{
    pointer++;

    if (pointer >= maxPoints)
    {
        return Point::CreateEmpty();
    }

    return Point(start[pointer]);
}


int Point::Max(Chan::E ch)
{
    return data[ch].byte0;
}


int Point::Min(Chan::E ch)
{
    return data[ch].byte1;
}


bool Point::IsEmpty()
{
    return (data[Chan::A].halfWord == 0 && data[Chan::B].halfWord == 0);
}


Point Point::CreateEmpty()
{
    return Point(BitSet16(0), BitSet16(0));
}


