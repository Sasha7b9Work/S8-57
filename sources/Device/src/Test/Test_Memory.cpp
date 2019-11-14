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

    Display::StartTest("Тест RAM");

    int numRecord = 4096;

    for (int i = 1; i <= numRecord; i++)
    {
        static int line = -1;

        if (i % 32 == 0)
        {
            line = Display::AddMessage(String("Запись %d из %d, %3.1f%%", i, numRecord, 100.0F * i / numRecord).CString(), line);
        }

        DataSettings ds;

        PrepareDS(&ds);

        ::RAM::PrepareForNewData(&ds);

        FillData(&ds);

        DataSettings *read = ::RAM::Read(std::rand() % ::RAM::NumberDatas());

        if (!CheckData(read))
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

    ::ROM::Data::EraseAll();

    int numRecord = 128;

    for (int i = 1; i <= numRecord; i++)
    {
        static int num = -1;

        num = Display::AddMessage(String("Запись %d из %d, %3.1f%%", i, numRecord, 100.0F * i / numRecord).CString(), num);

        uint numInROM = std::rand() % ::ROM::Data::MAX_NUM_SAVED_WAVES;

        DataSettings ds;

        ::ROM::Data::Save(numInROM, CreateDataInRAM(&ds));

        DataSettings *dsRead = nullptr;

        ::ROM::Data::Read(numInROM, &dsRead);

        if (!CheckData(dsRead))
        {
            return false;
        }

        continue;
    }
    
    Display::AddMessage("Cтираю память");

    ::ROM::Data::EraseAll();

    return true;
}


#define FILL(en, dat)                           \
    if (en) {                                   \
        uint8 *data = dat;                      \
        for (uint i = 0; i < numPoints; i++) {  \
            data[i] = static_cast<uint8>(i);    \
        }                                       \
    }

static void FillData(DataSettings *ds)
{
    uint numPoints = ds->SizeChannel();

    FILL(ds->enableA, ds->dataA);
    FILL(ds->enableB, ds->dataB);
}


#define CHECK(en, dat)                                           \
    if (en) {                                                    \
        uint8 *data = dat;                                       \
        for (uint i = 0; i < numPoints; i++)                     \
            if (data[i] != static_cast<uint8>(i)) return false;  \
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
    ds->enableB = ds->enableA ? static_cast<uint>(std::rand() % 2) : 0;
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
