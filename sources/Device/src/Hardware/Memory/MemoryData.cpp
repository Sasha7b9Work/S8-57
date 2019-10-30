#include "defines.h"
#include "Hardware/Memory/Memory.h"
#include "Hardware/HAL/HAL.h"


static void FillInfoFromSector(Sector::E /*numSector*/, bool /*existData*/[MAX_NUM_SAVED_WAVES])
{

}


void FlashMemory::Data::GetInfo(bool existData[MAX_NUM_SAVED_WAVES])
{
    /*
        Данные хранятся таким образом.
        Есть набор секторов, условно с 1-го по 5-й.
    */

    static const int NUM_SECTORS = 5;

    static const Sector::E sectors[5] = { Sector::_19_DATA_1, Sector::_20_DATA_2, Sector::_21_DATA_3, Sector::_22_DATA_4, Sector::_23_DATA_5 };

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
