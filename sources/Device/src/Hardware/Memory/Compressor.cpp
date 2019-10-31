#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Compressor.h"
#include "Data/DataSettings.h"


uint Compressor::GetPackedSize(const DataSettings *ds)
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


void Compressor::Pack(const DataSettings *ds, uint address)
{
    Packet packet = { STATE_VALID, static_cast<uint16>(GetPackedSize(ds)), TYPE_DATA };

    WriteToROM(&address, &packet, sizeof(packet));

    WriteToROM(&address, ds, sizeof(DataSettings));

    if (ds->enableA)
    {
        WriteToROM(&address, ds->dataA, ds->SizeChannel());
    }

    if (ds->enableB)
    {
        WriteToROM(&address, ds->dataB, ds->SizeChannel());
    }
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
    else if (IsErased() || IsValid())
    {
        return reinterpret_cast<Packet *>(reinterpret_cast<uint8 *>(const_cast<Packet *>(this)) + size);
    }
    else
    {
        LOG_WRITE("Ошибка следующего пакета");
    }

    return nullptr;
}
