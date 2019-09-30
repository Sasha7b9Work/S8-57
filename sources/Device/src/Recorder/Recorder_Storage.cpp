#include "defines.h"
#include "Recorder/Recorder_Storage.h"

#include "Hardware/Timer.h"
#include "Data/Heap.h"
#include "Hardware/Memory.h"


using namespace Recorder::Storage;


/// Обёртка для Heap()
namespace Stack
{
    static Frame frame;

    static void Push(const Frame &_frame)
    {
        frame = _frame;
        frame.SetDataAddress((uint16 *)ADDR_SECTOR_RECORDER_1); //-V566
    }

    static Frame *Top()
    {
        return &frame;
    }
}



uint Recorder::Storage::Frame::NumPoints()
{
    return numPoints;
}


int Recorder::Storage::Frame::FreeMemory()
{
    return 1024 * 128 - (int)(sizeof(Point) * numPoints);
}


void Recorder::Storage::Frame::AddPoint(BitSet16 dataA, BitSet16 dataB)
{
    BitSet32 bs(dataA.halfWord, dataB.halfWord);

    Memory::WriteData(ADDR_SECTOR_RECORDER_1 + numPoints * sizeof(Point), &bs, sizeof(BitSet32));

    numPoints++;
}


Recorder::Storage::Frame *Recorder::Storage::CurrentFrame()
{
    return Stack::Top();
}


void Recorder::Storage::CreateNewFrame()
{
    Frame frame;
    Stack::Push(frame);
}


void Recorder::Storage::Frame::SetDataAddress(uint16 *address)
{
    start = (Point *)address;
    numPoints = 0;
    pointer = MAX_UINT;
}


Point Frame::GetPoint(uint position, uint maxPoints)
{
    pointer = position - 1;

    return NextPoint(maxPoints);
}


Point Frame::NextPoint(uint maxPoints)
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


