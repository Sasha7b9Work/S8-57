#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Compressor.h"
#include "Hardware/Memory/Memory.h"



/// Поставить true в те элементы массива existData[], которые соотвествуют существующим в данном секторе записанным данным
static void FillInfoFromSector(const Sector *sector, bool existData[MAX_NUM_SAVED_WAVES])
{
    Packet *packet = reinterpret_cast<Packet *>(sector->address);

    while (packet && !packet->IsFree())
    {
        if (packet->IsData())
        {
            DataSettings *ds;
            if (packet->UnPack(&ds))
            {
                existData[ds->numROM] = true;
            }
        }
        packet = packet->Next();
    }
}


void FlashMemory::Data::GetInfo(bool existData[MAX_NUM_SAVED_WAVES])
{
    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        existData[i] = false;
    }

    static const int NUM_SECTORS = 5;

    static const Sector *sectors[5] =
    {
        &SECTOR(Sector::_19_DATA_1),
        &SECTOR(Sector::_20_DATA_2),
        &SECTOR(Sector::_21_DATA_3),
        &SECTOR(Sector::_22_DATA_4),
        &SECTOR(Sector::_23_DATA_5)
    };

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
