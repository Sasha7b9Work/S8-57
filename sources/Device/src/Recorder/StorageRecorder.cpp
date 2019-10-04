#include "defines.h"
#include "Recorder/StorageRecorder.h"
#include "Hardware/Timer.h"
#include "Data/Heap.h"
#include "Hardware/Memory.h"


/// Обёртка для Heap()
struct Stack
{
    static FrameRec frame;

    static void Push(const FrameRec &_frame)
    {
        frame = _frame;
        frame.SetDataAddress((uint16 *)ADDR_SECTOR_RECORDER_1); //-V566
    }

    static FrameRec *Top()
    {
        return &frame;
    }
};



uint FrameRec::NumPoints()
{
    return numPoints;
}


int FrameRec::FreeMemory()
{
    return 1024 * 128 - (int)(sizeof(PointRec) * numPoints);
}


void FrameRec::AddPoint(BitSet16 dataA, BitSet16 dataB)
{
    BitSet32 bs(dataA.halfWord, dataB.halfWord);

    EEPROM::WriteData(ADDR_SECTOR_RECORDER_1 + numPoints * sizeof(PointRec), &bs, sizeof(BitSet32));

    numPoints++;
}


FrameRec *StorageRecorder::CurrentFrame()
{
    return Stack::Top();
}


void StorageRecorder::CreateNewFrame()
{
    FrameRec frame;
    Stack::Push(frame);
}


void FrameRec::SetDataAddress(uint16 *address)
{
    start = (PointRec *)address;
    numPoints = 0;
    pointer = MAX_UINT;
}


PointRec FrameRec::GetPoint(uint position, uint maxPoints)
{
    pointer = position - 1;

    return NextPoint(maxPoints);
}


PointRec FrameRec::NextPoint(uint maxPoints)
{
    pointer++;

    if (pointer >= maxPoints)
    {
        return PointRec::CreateEmpty();
    }

    return PointRec(start[pointer]);
}


int PointRec::Max(Chan::E ch)
{
    return data[ch].byte0;
}


int PointRec::Min(Chan::E ch)
{
    return data[ch].byte1;
}


bool PointRec::IsEmpty()
{
    return (data[Chan::A].halfWord == 0 && data[Chan::B].halfWord == 0);
}


PointRec PointRec::CreateEmpty()
{
    return PointRec(BitSet16(0), BitSet16(0));
}


