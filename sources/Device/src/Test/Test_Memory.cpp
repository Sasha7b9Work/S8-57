#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Heap.h"
#include "Hardware/Memory/RAM.h"
#include "Hardware/Memory/ROM.h"
#include "Hardware/Memory/Sector.h"
#include "Settings/Settings.h"
#include "Test/Test.h"
#include <cstdlib>


static void PrepareDS(DataSettings *ds);
static void FillData(DataSettings *ds);
static bool CheckData(const DataSettings *ds);
/// Создаёт данные в RAM под индексом "0"
static DataSettings *CreateDataInRAM(DataSettings *ds);


bool Test::RAM::Test()
{
    ::RAM::Init();

    Display::StartTest("Тест ОЗУ");

    int numRecord = 5000;

    for (int i = 1; i <= numRecord; i++)
    {
        static int line = -1;

        if (i % 50 == 0)
        {
            line = Display::AddMessage(String("Запись %d из %d, %3.1f%%", i, numRecord, 100.0F * i / numRecord).CString(), line);
        }

        DataSettings ds;

        PrepareDS(&ds);

        ::RAM::PrepareForNewData(&ds);

        FillData(&ds);

        for (uint j = 0; j < ::RAM::NumberDatas(); j++)
        {
            DataSettings *read = ::RAM::Read(std::rand() % ::RAM::NumberDatas());

            if (read && !CheckData(read))
            {
                return false;
            }
        }
    }

    ::RAM::Init();

    return true;
}


bool Test::ROM::Data::Test()
{
    Display::StartTest("Тест ППЗУ");

    Display::AddMessage("Стираю память");

    ::ROM::Data::EraseAll();

    int numRecord = 300;

    for (int i = 1; i <= numRecord; i++)
    {
        static int num = -1;

        num = Display::AddMessage(String("Запись %d из %d, %3.1f%%", i, numRecord, 100.0F * i / numRecord).CString(), num);

        uint numInROM = std::rand() % ::ROM::Data::MAX_NUM_SAVED_WAVES;

        DataSettings ds;

        ::ROM::Data::Save(numInROM, CreateDataInRAM(&ds));

        for (uint j = 0; j < ::ROM::Data::MAX_NUM_SAVED_WAVES; j++)
        {
            const DataSettings *dsRead = ::ROM::Data::Read(j);

            if (dsRead)
            {
                if (!CheckData(dsRead))
                {
                    return false;
                }
            }
        }
    }
    
    Display::AddMessage("Cтираю память");

    ::ROM::Data::EraseAll();

    return true;
}


#define FILL(en, dat)                                       \
    if (en) {                                               \
        uint8 *data = dat;                                  \
        data[0] = static_cast<uint8>(std::rand());          \
        for (uint i = 1; i < numPoints; i++)                \
            { data[i] = static_cast<uint8>(data[0] * i); }  \
    }

static void FillData(DataSettings *ds)
{
    uint numPoints = ds->SizeChannel();

    FILL(ds->enableA, ds->dataA);
    FILL(ds->enableB, ds->dataB);
}


#define CHECK(en, dat)                                                              \
    if (en) {                                                                       \
        uint8 *data = dat;                                                          \
        for (uint i = 1; i < numPoints; i++)                                        \
            { if (data[i] != static_cast<uint8>(data[0] * i)) { return false; } }   \
    }


static bool CheckData(const DataSettings *ds)
{
    uint numPoints = ds->SizeChannel();

    CHECK(ds->enableA, ds->dataA);
    CHECK(ds->enableB, ds->dataB);

    return true;
}


static void PrepareDS(DataSettings *ds)
{
    ds->Fill();
    ds->enableA = static_cast<uint>(std::rand() % 2);
    ds->enableB = ds->enableA ? static_cast<uint>(std::rand() % 2) : 1;
    ds->peackDet = static_cast<uint>(PeakDetMode::Disabled);
    ds->enumPoints = static_cast<uint>(std::rand() % ENumPointsFPGA::Count);
}


static DataSettings *CreateDataInRAM(DataSettings *ds)
{
    PrepareDS(ds);
    ::RAM::PrepareForNewData(ds);
    FillData(ds);
    return ds;
}
