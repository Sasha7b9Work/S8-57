#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"
#include "Hardware/Memory/Memory.h"


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
/// ¬озвращает количество байт, занимаемых стЄртыми пакетами
static int GetNumberWornBytes(const Sector *sector);
/// —копировать данные из сектора src в сектор dest
//static void CopyData(const Sector *sectorDest, const Sector *sectorSrc);
/// —копировать данные в свободное место
static void CopyDataToFreeSpace(const Sector *src);
/// ѕереписать данные из сектора src в сектор dest
//static void CopyDataToSector(const Sector *dest, const Sector *src);
/// ¬озвращает указатель на первый пакет с данными из сектора sector и nullptr, если данных в секторе нет
static const Packet *GetFirstPacketWithData(const Sector *sector);
/// ¬озвращает указатель на первый пакет с данными за packet, или nullptr, если пакетов с данными в секторе больше нет
//static const Packet *GetNextPacketWithData(const Sector *sector, const Packet *packet);
/// ¬озвращает true, если в сектор не было записано ни одного пакета
static bool SectorIsFree(const Sector *sector);


void FlashMemory::Data::GetInfo(bool existData[MAX_NUM_SAVED_WAVES])
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


bool FlashMemory::Data::Read(int numInROM, DataSettings **ds)
{
    *ds = nullptr;

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        if (sectors[i]->ReadData(numInROM, ds))
        {
            return true;
        }
    }

    return false;
}


void FlashMemory::Data::Delete(int /*num*/)
{
}


void FlashMemory::Data::Save(int numInROM, const DataSettings *ds, uint8 *dataA, uint8 *dataB)
{
    Compress();

    const_cast<DataSettings *>(ds)->dataA = dataA;
    const_cast<DataSettings *>(ds)->dataB = dataB;

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


void FlashMemory::Data::DeleteAll()
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
        const Sector *src = GetMostWornSector();

        CopyDataToFreeSpace(src);

        src->Erase();
    }
}


static int NumberFreeSectors()
{
    int result = 0;

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        Packet *packet = reinterpret_cast<Packet *>(sectors[i]->address);

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
        Packet *packet = reinterpret_cast<Packet *>(sectors[i]->address);

        if (packet->IsFree())
        {
            return sectors[i];
        }
    }

    return nullptr;
}


static const Sector *GetMostWornSector()
{
    int numWornBytes = 0;               ///  оличество стЄртых байт 

    const Sector *sector = nullptr;

    for (int i = 0; i < NUM_SECTORS; i++)
    {
        int numBytes = GetNumberWornBytes(sectors[i]);

        if (numBytes > numWornBytes)
        {
            numWornBytes = numBytes;
            sector = sectors[i];
        }
    }

    return sector;
}


static int GetNumberWornBytes(const Sector *sector)
{
    int result = 0;

    Packet *packet = reinterpret_cast<Packet *>(sector->address);

    while (packet && !packet->IsFree())
    {
        if (packet->IsErased())
        {
            result += packet->Size();
        }

        packet = packet->Next();
    }

    return result;
}


//static void CopyData(const Sector *sectorDest, const Sector *sectorSrc)
//{
//    Packet *src = reinterpret_cast<Packet *>(sectorSrc->address);
//
//    Packet *dest = reinterpret_cast<Packet *>(sectorDest->address);
//
//    while (src && !src->IsFree())
//    {
//        if(!src->IsErased())
//        {
//            Compressor::Copy(dest, src);
//            dest = dest->Next();
//        }
//
//        src = src->Next();
//    }
//}


static void CopyDataToFreeSpace(const Sector *sectorSrc)
{
    /*
        ѕока есть свободное место в нестЄртых секторах, пишем туда.
        ѕотом пишем в стЄртые сектора.
    */

    const Packet *src = GetFirstPacketWithData(sectorSrc);

    if (!src)
    {
        return;
    }

    for (int i = 0; i < NUM_SECTORS; i++)                           // —начала производим запись в уже початые сектора
    {
        const Sector *sector = sectors[i];

        if (sector == sectorSrc || SectorIsFree(sector))
        {
            continue;
        }

        while (src)
        {
            if (!src->WriteToSector(sector))
            {
                break;
            }

            src = src->Next();
        }
    }
}


//static void CopyDataToSector(const Sector *, const Sector *sectorSrc)
//{
//    const Packet *src = GetFirstPacketWithData(sectorSrc);
//
//    if (!src)
//    {
//        return;
//    }
//
//    while (src)
//    {
//
//    }
//}


static const Packet *GetFirstPacketWithData(const Sector *sector)
{
    const Packet *result = reinterpret_cast<const Packet *>(sector->address);

    while (result && !result->IsFree())
    {
        if (result->IsValid())
        {
            return result;
        }

        result = result->Next();
    }

    return nullptr;
}


static bool SectorIsFree(const Sector *sector)
{
    Packet *packet = reinterpret_cast<Packet *>(sector->address);

    return packet->IsFree();
}
