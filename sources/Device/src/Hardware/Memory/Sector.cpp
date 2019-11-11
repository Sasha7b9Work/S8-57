#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"
#include "Data/DataSettings.h"


bool PacketROM::UnPack(DataSettings **ds) const
{
    if (!IsValid() || !IsData())
    {
        *ds = nullptr;
        return false;
    }

    *ds = const_cast<DataSettings *>(reinterpret_cast<const DataSettings *>(reinterpret_cast<const uint8 *>(this) + sizeof(PacketROM)));

    return true;
}


PacketROM *PacketROM::Next() const
{
    if (IsFree())
    {
        return nullptr;
    }

    return reinterpret_cast<PacketROM *>(reinterpret_cast<uint8 *>(const_cast<PacketROM *>(this)) + size);
}


uint PacketROM::Size() const
{
    if (IsFree())
    {
        return 0;
    }

    return size;
}


bool PacketROM::WriteToSector(const Sector *sector) const
{
    const PacketROM *dest = sector->FirstPacket();

    while (dest && !dest->IsFree())
    {
        dest = dest->Next();
    }

    uint addressWrite = reinterpret_cast<uint>(dest);      // По этому адресу будет производиться запись пакета

    uint lastAddress = addressWrite + size;                   // А это последний адрес записи

    if (lastAddress > sector->End())
    {
        return false;
    }

    HAL_ROM::WriteBufferBytes(addressWrite, this, size);

    return true;
}


void PacketROM::Erase() const
{
    uint data = STATE_ERASED;

    HAL_ROM::WriteBufferBytes(Address(), &data, 4);
}


uint PacketROM::GetPackedSize(const DataSettings *ds)
{
    return sizeof(PacketROM) +      // Packet
        sizeof(DataSettings) +      // DataSettings
        ds->NeedMemoryForData();    // data
}


uint Sector::End() const
{
    return address + size;
}


bool Sector::ExistPackets() const
{
    return !FirstPacket()->IsFree();
}


static void WriteToROM(uint *address, const void *data, uint size)
{
    HAL_ROM::WriteBufferBytes(*address, data, size);
    *address += size;
}


static void TranslateAddressToROM(const DataSettings *ds, const PacketROM *packet)
{
    uint8 *addressData = reinterpret_cast<uint8 *>(packet->Address() + sizeof(PacketROM) + sizeof(DataSettings)); // По этому адресу будут записаны данные первого из записываемых каналов

    if (ds->enableA)
    {
        const_cast<DataSettings *>(ds)->dataA = addressData;
        addressData += ds->SizeChannel();
    }

    if (ds->enableB)
    {
        const_cast<DataSettings *>(ds)->dataB = addressData;
    }
}


const PacketROM *Sector::WriteData(uint numInROM, const DataSettings *ds) const
{
    const PacketROM *packet = FirstFreePacket();

    if (packet == nullptr)
    {
        return nullptr;
    }

    uint recordAddress = packet->Address();

    uint end = End();

    uint sizePacket = sizeof(PacketROM);

    uint sizeDS = sizeof(DataSettings);

    uint sizeData = static_cast<uint>(ds->SizeChannel() * 2);

    volatile uint needSpace = sizePacket + sizeDS + sizeData;

    needSpace = needSpace;

    volatile uint existSpace = end - recordAddress;

    existSpace = existSpace;

    if (packet->Address() + PacketROM::GetPackedSize(ds) > End() - 10)      // Если пакет при записи вылезет за границу сектора. Даём запас - в конце сектора должны остаться миниму 4 байта,
    {                                                                    // помеченные 0xFFFFFFFF во избежание лишних проверок за выход за пределы сектора.
        return nullptr;
    }

    PacketROM record = { STATE_VALID, static_cast<uint16>(PacketROM::GetPackedSize(ds)), TYPE_DATA };

    WriteToROM(&recordAddress, &record, sizeof(record));

    const_cast<DataSettings *>(ds)->numInROM = numInROM;

    uint8 *addressDataA = ds->dataA;   // По этим адресам хранятся данные, подлежащие записи. Сохраним их перед тем, как в ds будут записаны новые - по которым данные будут храниться в ROM.
    uint8 *addressDataB = ds->dataB;   // По этим адресам хранятся данные, подлежащие записи. Сохраним их перед тем, как в ds будут записаны новые - по которым данные будут храниться в ROM.

    TranslateAddressToROM(ds, packet);

    WriteToROM(&recordAddress, ds, sizeof(DataSettings));

    if (ds->enableA)
    {
        WriteToROM(&recordAddress, addressDataA, ds->SizeChannel());
    }

    if (ds->enableB)
    {
        WriteToROM(&recordAddress, addressDataB, ds->SizeChannel());
    }

    return packet;
}


const PacketROM *Sector::FindValidPacket(uint numInROM) const
{
    const PacketROM *packet = FirstPacket();

    while (packet && !packet->IsFree())
    {
        if (packet->IsData())
        {
            DataSettings *settings;

            packet->UnPack(&settings);

            if (settings->numInROM == numInROM)
            {
                return packet;
            }
        }

        packet = packet->Next();
    }

    return nullptr;
}


const PacketROM *Sector::ReadData(uint numInROM, DataSettings **ds) const
{
    const PacketROM *packet = FindValidPacket(numInROM);

    if (packet)
    {
        packet->UnPack(ds);
        return packet;
    }

    return nullptr;
}


const PacketROM *Sector::DeleteData(uint numInROM) const
{
    const PacketROM *packet = FindValidPacket(numInROM);

    if (packet)
    {
        packet->Erase();
        return packet;
    }

    return nullptr;
}


const PacketROM *Sector::FirstPacket() const
{
    return reinterpret_cast<const PacketROM *>(address);
}


const PacketROM *Sector::FirstFreePacket() const
{
    const PacketROM *packet = FirstPacket();

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


void Sector::GetDataInfo(bool existData[ROM::Data::MAX_NUM_SAVED_WAVES]) const
{
    const PacketROM *packet = FirstPacket();

    while (packet && !packet->IsFree())
    {
        if (packet->IsData())
        {
            DataSettings *ds;
            if (packet->UnPack(&ds))
            {
                existData[ds->numInROM] = true;
            }
        }
        packet = packet->Next();
    }
}


uint Sector::GetNumberWornBytes() const
{
    uint result = 0;

    const PacketROM *packet = FirstPacket();

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


const PacketROM *Sector::GetFirstPacketWithData() const
{
    const PacketROM *result = FirstPacket();

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


int Sector::Number(uint address)
{
    for (int i = 0; i < Sector::Count; i++)
    {
        if (address < END_SECTOR(i))
        {
            return i;
        }
    }

    return -1;
}
