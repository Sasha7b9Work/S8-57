#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Heap.h"
#include "Hardware/Memory/MemoryROM.h"
#include "Hardware/Memory/Sector.h"
#include "Settings/Settings.h"
#include "Test/Test.h"
#include <cstdlib>


namespace Test
{
    namespace FlashMemory
    {
        namespace Data
        {
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
        }
    }
}


bool Test::FlashMemory::Data::Test()
{
    Display::StartTest("Тест флеш-памяти");

    Display::AddMessage("Стираю память");

    static uint totalMemory = 0;

    ::FlashMemory::Data::EraseAll();

    int numRecord = 8192;

    for (int i = 0; i < numRecord; i++)
    {
        static int num = -1;

        num = Display::AddMessage(String("Запись %d из %d, %3.1f%%", i, numRecord, 100.0F * i / numRecord).CString(), num);

        DataSettings ds;

        PrepareDS(&ds);

        uint numInROM = std::rand() % ::FlashMemory::Data::MAX_NUM_SAVED_WAVES;

        ::FlashMemory::Data::Save(numInROM, &ds);

        totalMemory += sizeof(Packet) + sizeof(DataSettings) + ds.SizeChannel() + ds.SizeChannel();

        DataSettings *dsRead = nullptr;

        ::FlashMemory::Data::Read(numInROM, &dsRead);

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

    ::FlashMemory::Data::EraseAll();

    Display::AddMessage("Завершено успешно");

    return true;
}
