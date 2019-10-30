#include "defines.h"
#include "Hardware/Memory/Memory.h"


static void FillInfoFromSector(uint /*address*/, bool /*existData*/[MAX_NUM_SAVED_WAVES])
{

}


void FlashMemory::Data::GetInfo(bool existData[MAX_NUM_SAVED_WAVES])
{
    /*
        Данные хранятся таким образом.
        Есть набор секторов, условно с 1-го по 5-й.
    */

    static const int NUM_SECTORS = 5;

    static const uint sectors[NUM_SECTORS] = { SEC_19_DATA_1, SEC_20_DATA_2, SEC_21_DATA_3, SEC_22_DATA_4, SEC_23_DATA_5 };

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        FillInfoFromSector(sectors[i], existData);
    }
}


bool FlashMemory::Data::Get(int /*num*/, DataSettings ** /*ds*/, uint8 ** /*dataA*/, uint8 ** /*dataB*/)
{
    return false;
}


void FlashMemory::Data::Delete(int /*num*/)
{
}


void FlashMemory::Data::Save(int /*num*/, const DataSettings * /*ds*/, uint8 * /*dataA*/, uint8 * /*dataB*/)
{
}


void FlashMemory::Data::DeleteAll()
{
}
