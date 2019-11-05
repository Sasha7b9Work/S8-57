#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"
#include "Data/DataSettings.h"


uint Packet::GetPackedSize(const DataSettings *ds)
{
    return sizeof(Packet) +         // Packet
        sizeof(DataSettings) +      // DataSettings
        ds->NeedMemoryForData();    // data
}


bool Packet::UnPack(DataSettings **ds) const
{
    if (!IsValid() || !IsData())
    {
        return false;
    }

    DataSettings *settings;

    uint8 *pointer = const_cast<uint8 *>(reinterpret_cast<const uint8 *>(this) + sizeof(Packet));

    settings = reinterpret_cast<DataSettings *>(pointer);

    settings = settings;

    *ds = reinterpret_cast<DataSettings *>(pointer);

    return true;
}


Packet *Packet::Next() const
{
    if (IsFree())
    {
        return nullptr;
    }

    return reinterpret_cast<Packet *>(reinterpret_cast<uint8 *>(const_cast<Packet *>(this)) + size);
}


uint Packet::Size() const
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
    const Packet *dest = sector->FirstPacket();

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

    FlashMemory::Write(addressWrite, this, size);

    return true;
}

void Packet::Erase() const
{
    uint data = STATE_ERASED;

    HAL_FLASH::WriteBufferBytes(Address(), &data, 4);
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
    HAL_FLASH::WriteBufferBytes(*address, data, size);
    *address += size;
}


static void TranslateAddressToROM(const DataSettings *ds, const Packet *packet)
{
    uint8 *addressData = reinterpret_cast<uint8 *>(packet->Address() + sizeof(Packet) + sizeof(DataSettings)); // По этому адресу будут записаны данные первого из записываемых каналов

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


const Packet *Sector::WriteData(uint numInROM, const DataSettings *ds) const
{
    const Packet *packet = FirstFreePacket();

    if (packet == nullptr)
    {
        return nullptr;
    }

    uint recordAddress = packet->Address();

    uint end = End();

    uint sizePacket = sizeof(Packet);

    uint sizeDS = sizeof(DataSettings);

    uint sizeData = static_cast<uint>(ds->SizeChannel() * 2);

    volatile uint needSpace = sizePacket + sizeDS + sizeData;

    needSpace = needSpace;

    volatile uint existSpace = end - recordAddress;

    existSpace = existSpace;

    if (packet->Address() + Packet::GetPackedSize(ds) > End() - 10)      // Если пакет при записи вылезет за границу сектора. Даём запас - в конце сектора должны остаться миниму 4 байта,
    {                                                                    // помеченные 0xFFFFFFFF во избежание лишних проверок за выход за пределы сектора.
        return nullptr;
    }

    Packet record = { STATE_VALID, static_cast<uint16>(Packet::GetPackedSize(ds)), TYPE_DATA };

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


const Packet *Sector::FindValidPacket(uint numInROM) const
{
    const Packet *packet = FirstPacket();

    while (packet && packet->Address() < End() && !packet->IsFree())
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


const Packet *Sector::ReadData(uint numInROM, DataSettings **ds) const
{
    const Packet *packet = FindValidPacket(numInROM);

    if (packet)
    {
        packet->UnPack(ds);
        return packet;
    }

    return nullptr;
}


const Packet *Sector::DeleteData(uint numInROM) const
{
    const Packet *packet = FindValidPacket(numInROM);

    if (packet)
    {
        packet->Erase();
        return packet;
    }

    return nullptr;
}


const Packet *Sector::FirstPacket() const
{
    return reinterpret_cast<const Packet *>(address);
}


const Packet *Sector::FirstFreePacket() const
{
    const Packet *packet = FirstPacket();

    while (packet && packet->Address() < End())
    {
        if (packet->IsFree())
        {
            return packet;
        }
        packet = packet->Next();
    }

    return nullptr;
}


void Sector::GetDataInfo(bool existData[FlashMemory::Data::MAX_NUM_SAVED_WAVES]) const
{
    const Packet *packet = FirstPacket();

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


int Sector::GetNumberWornBytes() const
{
    int result = 0;

    const Packet *packet = FirstPacket();

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


const Packet *Sector::GetFirstPacketWithData() const
{
    const Packet *result = FirstPacket();

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
