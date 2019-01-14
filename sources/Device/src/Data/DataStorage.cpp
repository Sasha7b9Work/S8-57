#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "DataBuffer.h"
#include "DataStorage.h"
#include "Reader.h"
#include "Utils/Math.h"
#include <string.h>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DataStorage::Init()
{
    DataBuffer::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataStorage::Push(DataSettings *ds)
{
    uint size = (uint)ds->SizeChannel();
    uint8 *dataA = ds->DataA();
    uint8 *dataB = ds->DataB();

    for (uint i = 0; i < size; i++)
    {
        LIMITATION(dataA[i], MIN_VALUE, MAX_VALUE);     // -V2516
        LIMITATION(dataB[i], MIN_VALUE, MAX_VALUE);     // -V2516
    }

    std::memcpy(OUT_A, ds->DataA(), (uint)ds->SizeChannel());
    std::memcpy(OUT_B, ds->DataB(), (uint)ds->SizeChannel());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint DataStorage::NumElementsInStorage()
{
    return DataBuffer::Size();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DataSettings *DataStorage::DataSettingsFromEnd(int /*fromEnd*/)
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *DataStorage::GetAverageData(Chan::E /*ch*/)
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataStorage::GetDataFromEnd(int /*fromEnd*/, DataSettings * /*ds*/, uint8 * /*dataA*/, uint8 * /*dataB*/)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DataStorage::GetLimitation(Chan::E /*ch*/, uint8 * /*data*/, int /*direction*/)
{
    return false;
}
