#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"
#include "Hardware/Memory/ROM.h"


static const int NUM_SECTORS = 5;

/// Ёти секторы предназначены дл€ хранени€ данных
static const Sector *sectors[5] =
{
    &SECTOR(Sector::_19_DATA_1),
    &SECTOR(Sector::_20_DATA_2),
    &SECTOR(Sector::_21_DATA_3),
    &SECTOR(Sector::_22_DATA_4),
    &SECTOR(Sector::_23_DATA_5)
};


/// —жать хранимые данные - перераспределить их по секторам таким образом, чтобы осталось полностью свободными как мимиум два сектора
static void Compress();
/// ¬озвращает количество полностью свободных дл€ записи секторов
static int NumberFreeSectors();
/// ¬озвращает указатель на первый свободный сектор
static const Sector *GetFirstFreeSector();
/// ¬озвращает указатель на наиболее потЄртый сектор - где стЄртые пакеты занимают больше всего места
static const Sector *GetMostWornSector();
/// —копировать данные в свободное место
static void CopyDataToFreeSpace(const Sector *src);


static void LogAllSectors();


void ROM::Data::GetInfo(bool existData[MAX_NUM_SAVED_WAVES])
{
    for (int i = 0; i < MAX_NUM_SAVED_WAVES; i++)
    {
        existData[i] = false;
    }

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        sectors[i]->GetDataInfo(existData);
    }
}


const DataSettings *ROM::Data::Read(uint numInROM)
{
    for (int i = 0; i < NUM_SECTORS; i++)
    {
        const DataSettings *ds = sectors[i]->ReadData(numInROM);

        if (ds)
        {
            return ds;
        }
    }

    return nullptr;
}


void ROM::Data::Erase(uint numInROM)
{
    for (int i = 0; i < NUM_SECTORS; i++)
    {
        if (sectors[i]->DeleteData(numInROM))
        {
            return;
        }
    }
}


void ROM::Data::Save(uint numInROM, const DataSettings *ds)
{
    Erase(numInROM);

    Compress();

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        const Sector *sector = sectors[i];

        if (sector->ExistPackets())                 // ѕишем только в сектор, в который уже производилась запись
        {
            if (sector->WriteData(numInROM, ds))
            {
                return;
            }
        }
    }

    // ≈сли места в частично зан€тых секторах нет, будем записывать в свободный

    GetFirstFreeSector()->WriteData(numInROM, ds);
}


void ROM::Data::EraseAll()
{
    for (int i = 0; i < NUM_SECTORS; i++)
    {
        sectors[i]->Erase();
    }
}


static void Compress()
{
    while (NumberFreeSectors() < 2)
    {
        LogAllSectors();

        if (Sector::needLog)
        {
            LOG_WRITE("");
            LOG_WRITE("ƒелаю компрессию");
        }

        const Sector *src = GetMostWornSector();

        CopyDataToFreeSpace(src);

        src->Erase();

        LogAllSectors();
    }
}


static int NumberFreeSectors()
{
    int result = 0;

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        const PacketROM *packet = sectors[i]->FirstPacket();

        if (packet->IsFree())
        {
            result++;
        }
    }

    return result;
}


static const Sector *GetFirstFreeSector()
{
    for (int i = 0; i < NUM_SECTORS; i++)
    {
        const PacketROM *packet = sectors[i]->FirstPacket();

        if (packet->IsFree())
        {
            return sectors[i];
        }
    }

    return nullptr;
}


static const Sector *GetMostWornSector()
{
    uint numWornBytes = 0;               ///  оличество стЄртых байт 

    const Sector *sector = nullptr;

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        uint numBytes = sectors[i]->GetNumberWornBytes();

        if (numBytes > numWornBytes)
        {
            numWornBytes = numBytes;
            sector = sectors[i];
        }
    }

    return sector;
}


static void CopyDataToFreeSpace(const Sector *sectorSrc)
{
    /*
        ѕока есть свободное место в нестЄртых секторах, пишем туда.
        ѕотом пишем в стЄртые сектора.
    */

    if (Sector::needLog)
    {
        LOG_WRITE(" опирую данные из сектора %d", sectorSrc->number);
    }

    const PacketROM *src = sectorSrc->GetFirstPacketWithData();

    if (!src)
    {
        return;
    }

    for (int i = 0; i < NUM_SECTORS; i++)                           // —начала производим запись в уже початые сектора
    {
        if (src->IsFree())
        {
            break;
        }

        const Sector *sector = sectors[i];

        if (sector == sectorSrc || !sector->ExistPackets())
        {
            continue;
        }

        while (src)
        {
            if (src->IsData())
            {
                if (!src->WriteToSector(sector))
                {
                    break;
                }
            }

            do 
            {
                src = src->Next();

            } while (src->IsErased());

            if (src->IsFree())
            {
                break;
            }
        }
    }
}


static void LogAllSectors()
{
    for (int i = 0; i < NUM_SECTORS; i++)
    {
        sectors[i]->Log();
    }
}
