#include "defines.h"
#include "log.h"
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


void Record::AddPoint(BitSet16 dataA, BitSet16 dataB)
{
    LOG_WRITE("добавлена точка %d %d %d %d", dataA.byte0, dataA.byte1, dataB.byte0, dataB.byte1);
}


Record *StorageRecorder::CurrentRecord()
{
    static Record record;

    return &record;
}


void StorageRecorder::CreateNewRecord()
{
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
