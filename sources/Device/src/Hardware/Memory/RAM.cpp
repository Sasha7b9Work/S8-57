#include "defines.h"
#include "Data/Heap.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/StorageOsci.h"
#include "Utils/Math.h"
#include <cstring>


struct Packet;


#define BEGIN reinterpret_cast<uint>(Heap::Begin())
#define END   reinterpret_cast<uint>(Heap::End())


int16 RAM::currentSignal = 0;
/// Указатель на самый старый записанный пакет. Он будет стёрт первым
Packet *oldest = reinterpret_cast<Packet *>(BEGIN);
/// Указатель на последний записанный пакет. Он будет стёрт последним
Packet *newest = nullptr;

/// Удалить самую старую запись
static void RemoveOldest();
/// Освободить место для записи пакета с данными в соответствии с ds
static uint AllocateMemoryForPacket(const DataSettings *ds);
/// Освободить size байт памяти с начала буфера
static void AllocateMemoryFromBegin(uint size);
/// Записывает по адресу dest. Возвращает адрес первого байта после записи
static uint *WriteToRAM(uint *dest, const void *src, uint size)
{
    std::memcpy(dest, src, size);
    
    return (dest + size);
}


struct Packet
{
    /// Адрес следующего пакета, более "свежего". Если addrNext == 0x00000000, следующего пакета нет, этот пакет самый новый
    uint addrNewest;

    /// Упаковать данные по адресу this. Возвращает указатель на пакет, следующий за ним
    Packet *Pack(const DataSettings *ds)
    {
        DataSettings data = *ds;
        data.dataA = data.dataB = nullptr;

        addrNewest = 0x0000000;                                                                         // Указываем, что это самый последний пакет

        uint *address = reinterpret_cast<uint *>(Address() + sizeof(Packet));                           // По этому адресу запишем DataSettings

        address = WriteToRAM(address, ds, sizeof(DataSettings));                                        // Записываем DataSettings

        if (ds->enableA)                                                                                // Записываем данные первого канала
        {
            data.dataA = reinterpret_cast<uint8 *>(address);
            address = WriteToRAM(address, ds->dataA, ds->SizeChannel());
        }

        if (ds->dataB)                                                                                  // Записываем данные второго канала
        {
            data.dataB = reinterpret_cast<uint8 *>(address);
            WriteToRAM(address, ds->dataB, ds->SizeChannel());
        }

        std::memcpy(reinterpret_cast<uint *>(Address() + sizeof(Packet)), &data, sizeof(DataSettings)); // Записываем скорректированные настройки

        return reinterpret_cast<Packet *>(addrNewest);
    }

    uint Address() const
    {
        return reinterpret_cast<uint>(this);
    };
    /// Возвращает размер памяти, необходимой для хранения данных в соответсвии с настройками ds
    static uint NeedMemoryForPacedData(const DataSettings *ds)
    {
        return sizeof(Packet) + sizeof(DataSettings) + ds->NeedMemoryForData();
    }

    uint Size() const
    {
        return sizeof(Packet) + sizeof(DataSettings) + GetDataSettings()->NeedMemoryForData();
    }

    DataSettings *GetDataSettings() const
    {
        return reinterpret_cast<DataSettings *>(Address() + sizeof(Packet));
    }
};


void RAM::Init()
{
    oldest = reinterpret_cast<Packet *>(BEGIN);
    newest = nullptr;
}


void RAM::Save(const DataSettings *ds)
{
    uint address = AllocateMemoryForPacket(ds);         // Находим адрес для записи нового пакета

    newest->addrNewest = address;                       // Указываем его в качестве адреса следующего пакета для предыдущего

    newest = reinterpret_cast<Packet *>(address);       // Устанавилваем этот адрес в качестве новейшего пакета

    newest->Pack(ds);                                   // И упаковываем данные
}


bool RAM::Read(DataSettings **, uint)
{
    return false;
}


uint RAM::NumberDatas()
{
    return 0;
}


static uint AllocateMemoryForPacket(const DataSettings *ds)
{
    if (newest == nullptr)                                                  // Ещё нет ни одной записи
    {                                                                       
        return BEGIN;                                                       
    }                                                                       

    uint addrFirst = newest->Address() + newest->Size();                    // По этому адресу должна быть следующая запись
    uint addrLast = addrFirst + Packet::NeedMemoryForPacedData(ds);         // А это последний байт следующей записи
                                                                            
    if (newest > oldest)                                                    // Нормальный порядок следования - более новые записи расположены после более старых
    {                                                                       
        if (addrLast < END)                                                 // Если хватает памяти для записи
        {
            return addrFirst;
        }
        else                                                                // Если остатка памяти не хватает для сохранения записи
        {
            AllocateMemoryFromBegin(Packet::NeedMemoryForPacedData(ds));    // То освобождаем в начале памяти необходимое число байт для хранения пакета

            return BEGIN;
        }
    }

    if (newest < oldest)                                                    // Более старые записи расположены после более новых
    {
        while (addrLast >= reinterpret_cast<uint>(oldest))
        {
            RemoveOldest();
        }
    }

    return addrFirst;
}


static void AllocateMemoryFromBegin(uint size)
{
    while (oldest->Address() - BEGIN < size)
    {
        RemoveOldest();
    }
}


static void RemoveOldest()
{
    oldest = reinterpret_cast<Packet *>(oldest->addrNewest);
}
