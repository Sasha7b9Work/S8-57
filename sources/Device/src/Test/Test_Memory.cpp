#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Heap.h"
#include "Data/Reader.h"
#include "Hardware/Memory/RAM.h"
#include "Hardware/Memory/ROM.h"
#include "Hardware/Memory/Sector.h"
#include "Settings/Settings.h"
#include "Test/Test.h"
#include <cstdlib>


static void FillData(uint8 *dataA, uint8 *dataB, uint numPoints)
{
    for (uint i = 0; i < numPoints; i++)
    {
        //dataA[i] = static_cast<uint8>(std::rand());
        //dataB[i] = static_cast<uint8>(std::rand());

        dataA[i] = static_cast<uint8>(i);
        dataB[i] = static_cast<uint8>(i);
    }
}

static bool CheckData(uint8 *dataA, uint8 *dataB, uint numPoints)
{
    for (uint i = 0; i < numPoints; i++)
    {
        if (dataA[i] != static_cast<uint8>(i))
        {
            return false;
        }

        if (dataB[i] != static_cast<uint8>(i))
        {
            return false;
        }

        //if (dataA[i] != OUT_A[i])
        //{
        //    return false;
        //}
        //
        //if (dataB[i] != OUT_B[i])
        //{
        //    return false;
        //}
    }

    return true;
}

static void PrepareDS(DataSettings *ds)
{
    ds->Fill(0, 0);

    ds->peackDet = static_cast<uint>(PeakDetMode::Disabled);
    ds->enumPoints = static_cast<uint>(std::rand() % ENumPointsFPGA::Count);

    uint8 *dataA = OUT_A;
    uint8 *dataB = OUT_B;
    
    FillData(dataA, dataB, ds->SizeChannel());

    ds->dataA = dataA;
    ds->dataB = dataB;
}


bool Test::RAM::Test()
{
    ::RAM::Init();

    Display::StartTest("Тест RAM");

    int numRecord = 1024;

    for (int i = 0; i < numRecord; i++)
    {
        static int line = -1;
       
        line = Display::AddMessage(String("Запись %d из %d, %3.1f%%", i, numRecord, 100.0F * i / numRecord).CString(), line);

        DataSettings ds;

        PrepareDS(&ds);

        ::RAM::Save(&ds);

        DataSettings *read;

        ::RAM::Read(&read);

        if (!CheckData(read->dataA, read->dataB, read->SizeChannel()))
        {
            return false;
        }

        continue;
    }

    ::RAM::Init();

    return true;
}


bool Test::ROM::Data::Test()
{
    Display::StartTest("Тест ROM");

    Display::AddMessage("Стираю память");

    static uint totalMemory = 0;

    ::ROM::Data::EraseAll();

    int numRecord = 128;

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

        if (!CheckData(dsRead->dataA, dsRead->dataB, dsRead->SizeChannel()))
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
