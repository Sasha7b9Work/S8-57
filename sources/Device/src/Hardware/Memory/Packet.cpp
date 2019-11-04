#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Packet.h"
#include "Data/DataSettings.h"


uint Packet::GetPackedSize(const DataSettings *ds)
{
    return sizeof(Packet) +         // Packet
        sizeof(DataSettings) +      // DataSettings
        ds->NeedMemoryForData();    // data
}


static void WriteToROM(uint *address, const void *data, int size)
{
    HAL_FLASH::WriteBufferBytes(*address, data, size);
    *address += size;
}


bool Packet::UnPack(DataSettings **ds) const
{
    if (!IsValid() || !IsData())
    {
        return false;
    }

    uint8 *pointer = const_cast<uint8 *>(reinterpret_cast<const uint8 *>(this) + sizeof(Packet));

    *ds = reinterpret_cast<DataSettings *>(pointer);

    return true;
}


Packet *Packet::Next() const
{
    if (IsFree())
    {
        return nullptr;
    }

    Packet *result = reinterpret_cast<Packet *>(reinterpret_cast<uint8 *>(const_cast<Packet *>(this)) + size);

    return result;
}


int Packet::Size() const
{
    if (IsFree())
    {
        return 0;
    }

    return size;
}


void Compressor::Copy(Packet *dest, const Packet *src)
{
    FlashMemory::Write(reinterpret_cast<uint>(dest), const_cast<const Packet *>(src), src->Size());
}


bool Packet::WriteToSector(const Sector *sector) const
{
    Packet *dest = reinterpret_cast<Packet *>(sector->address);

    while (dest && !dest->IsFree())
    {
        dest = dest->Next();
    }

    uint addressWrite = reinterpret_cast<uint>(dest);      // ѕо этому адресу будет производитьс€ запись пакета

    uint lastAddress = addressWrite + size;                   // ј это последний адрес записи

    if (lastAddress > sector->End())
    {
        return false;
    }

    FlashMemory::Write(addressWrite, this, size);

    return true;
}
