#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Heap.h"
#include "Hardware/Memory/Memory.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Test/Test.h"
#include <cstdlib>


bool Test::Run()
{
    FlashMemory::Data::Test();
    
    return false;
}


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
        }
    }
}


bool Test::FlashMemory::Data::Test()
{
    ::FlashMemory::Data::DeleteAll();

    for (int i = 0; i < 1000; i++)
    {
        ENumPointsFPGA enumPoints(static_cast<ENumPointsFPGA::E>(std::rand() % ENumPointsFPGA::Count));

        uint sizeChannel = enumPoints.BytesInChannel(PeakDetMode::Disabled);

        uint8 *dataA = static_cast<uint8 *>(Heap::Begin());
        uint8 *dataB = static_cast<uint8 *>(Heap::Begin() + sizeChannel);

        FillData(dataA, sizeChannel);
        FillData(dataB, sizeChannel);

        DataSettings ds;
        ds.Fill(dataA, dataB);

        uint numInROM = std::rand() % ::FlashMemory::Data::MAX_NUM_SAVED_WAVES;

        ::FlashMemory::Data::Save(numInROM, &ds);

        DataSettings *dsRead = nullptr;

        static int counter = 0;

        counter++;

        if (counter == 2)
        {
            counter = counter;
        }

        ::FlashMemory::Data::Read(numInROM, &dsRead);

        if (!Compare(dataA, dsRead->dataA, sizeChannel))
        {
            return false;
        }

        if (!Compare(dataB, dsRead->dataB, sizeChannel))
        {
            return false;
        }

        continue;
    }

    return true;
}
