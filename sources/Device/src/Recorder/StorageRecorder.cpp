#include "defines.h"
#include "log.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/ExtRAM.h"
#include "Recorder/StorageRecorder.h"
#include "Settings/Settings.h"


uint Record::NumPoints() const
{
    return numPoints;
}


void Record::AddPoint(BitSet16 dataA, BitSet16 dataB)
{
    LOG_WRITE("добавлена точка %d %d %d %d", dataA.byte0, dataA.byte1, dataB.byte0, dataB.byte1);
}


void Record::Init()
{
    timeStart = HAL_RTC::GetPackedTime();
    numPoints = 0;
    sources = 0;
    bytesOnPoint = 0;

    if(set.rec.enA)
    {
        _SET_BIT(sources, 0);
        bytesOnPoint += 2;
    }

    if(set.rec.enB)
    {
        _SET_BIT(sources, 1);
        bytesOnPoint += 2;
    }

    if(set.rec.enSensor)
    {
        _SET_BIT(sources, 2);
        bytesOnPoint += sizeof(float) * 2;      //  ажда€ точка датчика требует 2 значени€ типа float - минимальное и максимальное
    }
}


uint Record::FreeMemory() const
{
    return 5;
}


const uint8 *Record::Begin() const
{
    return reinterpret_cast<const uint8 *>(this);
}


const uint8 *Record::End() const
{
    if(!IsValid())
    {
        return nullptr;
    }

    return Begin() + sizeof(*this) + bytesOnPoint * numPoints;
}


bool Record::IsValid() const
{
    if(Begin() < ExtRAM::Begin() || End() > ExtRAM::End())
    {
        return false;
    }

    if(timeStart.year == 0 || sources == 0 || bytesOnPoint < 2)
    {
        return false;
    }

    return true;
}


Record *StorageRecorder::CurrentRecord()
{
    static Record record;

    return &record;
}


void StorageRecorder::CreateNewRecord()
{

}


void StorageRecorder::Init()
{
    ExtRAM::Fill();
}


uint StorageRecorder::NumRecords()
{
    const Record *record = reinterpret_cast<Record *>(ExtRAM::Begin());

    uint result = 0;

    while(record->IsValid())
    {
        record = reinterpret_cast<const Record *>(record->End());
        result++;
    }

    return result;
}
