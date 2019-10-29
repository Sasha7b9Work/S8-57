#include "defines.h"
#include "Recorder/StorageRecorder.h"
#include "Hardware/Timer.h"
#include "Data/Heap.h"
#include "Hardware/Memory/Memory.h"


/// Обёртка для Heap()
struct Stack
{
    static Record record;

    static void Push(const Record &_record)
    {
        record = _record;
        record.SetDataAddress((uint16 *)S17_RECORDER_1); //-V566
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

    EEPROM::WriteData(S17_RECORDER_1 + numPoints * sizeof(Point), &bs, sizeof(BitSet32));

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
    startMC = reinterpret_cast<Point *>(address);
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

#ifdef GUI

    Point *address = reinterpret_cast<Point *>(Address::FromMC(reinterpret_cast<uint>(startMC)).ToPC());
    return Point(address[pointer]);

#else

    return Point(startMC[pointer]);

#endif
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


