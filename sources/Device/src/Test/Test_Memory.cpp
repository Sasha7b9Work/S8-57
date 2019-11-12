#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Heap.h"
#include "Hardware/Memory/RAM.h"
#include "Hardware/Memory/ROM.h"
#include "Hardware/Memory/Sector.h"
#include "Settings/Settings.h"
#include "Test/Test.h"
#include <cstdlib>


static void FillData(uint8 *data, uint numPoints)
{
    for (uint i = 0; i < numPoints; i++)
    {
        data[i] = static_cast<uint8>(i);
    }
}

static bool Compare(uint8 *src, uint8 *dest, uint numPoints)
{
    for (uint i = 0; i < numPoints; i++)
    {
        if (*src++ != *dest++)
        {
            return false;
        }
    }

    return true;
}

static void PrepareDS(DataSettings *ds)
{
    uint8 *dataA = static_cast<uint8 *>(Heap::Begin());
    uint8 *dataB = static_cast<uint8 *>(Heap::Begin() + ds->SizeChannel());

    FillData(dataA, ds->SizeChannel());
    FillData(dataB, ds->SizeChannel());

    ds->Fill(dataA, dataB);

    ds->enumPoints = static_cast<uint>(std::rand() % ENumPointsFPGA::Count);
    ds->peackDet = PeakDetMode::Disabled;
}


bool Test::RAM::Test()
{
    Display::StartTest("Тест RAM");

    int numRecord = 8192;

    for (int i = 0; i < numRecord; i++)
    {
        static int line = -1;
        
        line = Display::AddMessage(String("Запись %d из %d, %3.1f%%", i, numRecord, 100.0F * i / numRecord).CString(), line);

        DataSettings ds;

        PrepareDS(&ds);

        ::RAM::Save(&ds);

        DataSettings *read;

        ::RAM::Read(&read);

        if (!Compare(ds.dataA, read->dataA, ds.SizeChannel()))
        {
            return false;
        }

        if (!Compare(ds.dataB, read->dataB, ds.SizeChannel()))
        {
            return false;
        }

        continue;
    }

    return true;
}


bool Test::ROM::Data::Test()
{
    Display::StartTest("Тест ROM");

    Display::AddMessage("Стираю память");

    static uint totalMemory = 0;

    ::ROM::Data::EraseAll();

    int numRecord = 128000;

    for (int i = 0; i < numRecord; i++)
    {
        static int num = -1;

        num = Display::AddMessage(String("Запись %d из %d, %3.1f%%", i, numRecord, 100.0F * i / numRecord).CString(), num);

        DataSettings ds;

        PrepareDS(&ds);

        uint numInROM = std::rand() % ::ROM::Data::MAX_NUM_SAVED_WAVES;

        ::ROM::Data::Save(numInROM, &ds);

        totalMemory += sizeof(PacketROM) + sizeof(DataSettings) + ds.SizeChannel() + ds.SizeChannel();

        DataSettings *dsRead = nullptr;

        ::ROM::Data::Read(numInROM, &dsRead);

        if (!Compare(ds.dataA, dsRead->dataA, ds.SizeChannel()))
        {
            return false;
        }

        if (!Compare(ds.dataB, dsRead->dataB, ds.SizeChannel()))
        {
            return false;
        }

        continue;
    }
    
    Display::AddMessage("Cтираю память");

    ::ROM::Data::EraseAll();

    Display::AddMessage("Завершено успешно");

    return true;
}
