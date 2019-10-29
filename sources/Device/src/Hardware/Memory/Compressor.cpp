#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Compressor.h"
#include "Data/DataSettings.h"


struct Packet
{
    uint  state;    // Состояние пакета:
                    // 0xFFFFFFFF - в пакет запись не производилась
                    // 0x00000000 - пакет стёрт
                    // 0xFF00FF00 - действующий пакет
    uint16 size;    // Размер пакета - sizeof(size) + sizeof(type) + sizeof(data)
    uint16 type;    // Тип данных :
                    // 0 - DataSettings
};


#define FREE    0xFFFFFFFFU
#define ERASED  0x00000000U
#define VALID   0xFF00FF00U

#define TYPE_DATA 0U


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


void Compressor::PackToROM(const DataSettings *ds, Address address)
{
    Packet packet = { VALID, static_cast<uint16>(GetPackedSize(ds)), TYPE_DATA };

    uint addr = address.address;

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
    Packet packet = *reinterpret_cast<Packet *>(address.address);

    if (packet.state != VALID || packet.type != TYPE_DATA)
    {
        return false;
    }

    **ds = *reinterpret_cast<DataSettings *>(address.address + sizeof(Packet));

    return true;
}
