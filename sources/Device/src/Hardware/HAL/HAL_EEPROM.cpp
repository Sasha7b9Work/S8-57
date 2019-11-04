#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Packet.h"


uint Sector::End() const
{
    return address + size;
}


bool Sector::NotExistPackets() const
{
    Packet *packet = reinterpret_cast<Packet *>(address);

    return packet->IsFree();
}


static void WriteToROM(uint *address, const void *data, int size)
{
    HAL_FLASH::WriteBufferBytes(*address, data, size);
    *address += size;
}


const Packet *Sector::WritePacket(const DataSettings *ds) const
{
    Packet *firstPacket = GetFirstFreePacket();

    if (firstPacket == nullptr)
    {
        return nullptr;
    }

    uint packetAddress = firstPacket->Address();

    if (firstPacket->Address() + Packet::GetPackedSize(ds) > End())      // ≈сли пакет при записи вылезет за границу сектора
    {
        return nullptr;
    }

    Packet packet = { STATE_VALID, static_cast<uint16>(Packet::GetPackedSize(ds)), TYPE_DATA };

    WriteToROM(&packetAddress, &packet, sizeof(packet));

    WriteToROM(&packetAddress, ds, sizeof(DataSettings));

    if (ds->enableA)
    {
        WriteToROM(&packetAddress, ds->dataA, ds->SizeChannel());
    }

    if (ds->enableB)
    {
        WriteToROM(&packetAddress, ds->dataB, ds->SizeChannel());
    }

    return firstPacket;
}


Packet *Sector::GetFirstFreePacket() const
{
    Packet *packet = reinterpret_cast<Packet *>(address);

    while (packet)
    {
        if (packet->IsFree())
        {
            return packet;
        }
        packet = packet->Next();
    }

    return nullptr;
}
