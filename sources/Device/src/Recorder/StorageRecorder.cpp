#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/ExtRAM.h"
#include "Recorder/StorageRecorder.h"



uint Record::NumPoints()
{
    return numPoints;
}


int Record::FreeMemory()
{
    return 1024 * 128 - static_cast<int>(sizeof(Point) * numPoints);
}


void Record::AddPoint(BitSet16, BitSet16)
{
}


Record *StorageRecorder::CurrentRecord()
{
    return nullptr;
}


void StorageRecorder::CreateNewRecord()
{
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


void StorageRecorder::Init()
{
    ExtRAM::Fill();
}
