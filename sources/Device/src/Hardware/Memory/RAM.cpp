#include "defines.h"
#include "Data/Heap.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/StorageOsci.h"
#include "Utils/Math.h"
#include <cstring>



struct Packet
{
    /*
        Данные хранятся таким образом
    */
    uint addrNewest;    /// Адрес следующего пакета, более "свежего"
                        /// addrNext == 0x00000000 - в пакете ничего не записано
                        /// addrNext == 0xffffffff - в пакете записаны данные, но это последний пакет
    /// Упаковать данные по адресу this. Возвращает указатель на пакет, следующий за ним
    Packet *Pack(const DataSettings *)
    {
        if (IsEmpty())
        {

        }

        return nullptr;
    }
    /// Упаковать данные после данного пакета. Возвращает указатель на упкованный пакет, котоырй становится самым новым
    Packet *PackNewest(const DataSettings *)
    {
        return nullptr;
    }
    /// Возвращает указатель на следующий пакет
    Packet *Next() const
    {
        return nullptr;
    }
    /// Возвращает true, если пакет пустой (size == 0x0000)
    bool IsEmpty() const
    {
        return (addrNewest == 0x0000000);
    }

    uint Size() const
    {
        uint result = 0;

        if (!IsEmpty())
        {
            result = sizeof(Packet) + sizeof(DataSettings) + GetDataSettings()->NeedMemoryForData();
        }

        return result;
    }

    DataSettings *GetDataSettings() const
    {
        if (IsEmpty())
        {
            return nullptr;
        }

        return reinterpret_cast<DataSettings *>(Address() + sizeof(Packet));
    }

    uint Address() const { return reinterpret_cast<uint>(this); };
};


int16 RAM::currentSignal = 0;

/// Указатель на самый старый записанный пакет. Он будет стёрт первым
Packet *oldest = reinterpret_cast<Packet *>(Heap::Begin());
/// Указатель на последний записанный пакет. Он будет стёрт последним
Packet *newest = reinterpret_cast<Packet *>(Heap::Begin());


void RAM::Init()
{
    std::memset(Heap::Begin(), 0x00, 4);
}


void RAM::Save(const DataSettings *ds)
{
    Packet *packet = newest;

    if(packet->IsEmpty())
    {
        packet->Pack(ds);
    }
    else
    {
        packet->PackNewest(ds);
    }
}


bool RAM::Read(DataSettings **, uint)
{
    return false;
}


uint RAM::NumberDatas()
{
    return 0;
}
