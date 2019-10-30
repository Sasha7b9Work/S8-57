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
    HAL_EEPROM::WriteBufferBytes(*address, data, size);
    *address += size;
}


void Compressor::Pack(const DataSettings *ds, Address address)
{
    Packet packet = { STATE_VALID, static_cast<uint16>(GetPackedSize(ds)), TYPE_DATA };

    uint addr = address.addressMC;

    WriteToROM(&addr, &packet, sizeof(packet));

    WriteToROM(&addr, ds, sizeof(DataSettings));

    if (ds->enableA)
    {
        WriteToROM(&addr, ds->dataA, ds->SizeChannel());
    }

    if (ds->enableB)
    {
        WriteToROM(&addr, ds->dataB, ds->SizeChannel());
    }
}


bool Compressor::UnPack(Address address, DataSettings **ds)
{
    Packet packet = *reinterpret_cast<Packet *>(address.addressMC);

    if (packet.state != STATE_VALID || packet.type != TYPE_DATA)
    {
        return false;
    }

    **ds = *reinterpret_cast<DataSettings *>(address.addressMC + sizeof(Packet));

    return true;
}


Packet *Compressor::NextPacket(Packet *packet)
{
    if (packet->state == STATE_FREE)
    {
        return nullptr;
    }
    else if (packet->state == STATE_ERASED || packet->state == STATE_VALID)
    {
        return reinterpret_cast<Packet *>(reinterpret_cast<uint8 *>(packet) + packet->size);
    }
    else
    {
        LOG_WRITE("Ошибка следующего пакета");
    }

    return nullptr;
}
