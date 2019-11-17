#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Heap.h"
#include "Hardware/Memory/RAM.h"
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
    uint8 *address = reinterpret_cast<uint8 *>(dest);

    std::memcpy(address, src, size);
    
    return reinterpret_cast<uint *>(address + size);
}


struct Packet
{
    /// Адрес следующего пакета, более "свежего". Если addrNext == 0x00000000, следующего пакета нет, этот пакет самый новый
    uint addrNewest;

    /// Упаковать данные по адресу this. Возвращает указатель на пакет, следующий за ним
    void Pack(const DataSettings *ds)
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
    }
    /// Подготовить пакет для сохранения данных в соответствии с настройками ds
    void Prepare(DataSettings *ds)
    {
        addrNewest = 0x00000000;
        uint *address = reinterpret_cast<uint *>(Address() + sizeof(Packet));

        ds->dataA = ds->dataB = nullptr;
        uint8 *addrData = reinterpret_cast<uint8 *>(reinterpret_cast<uint8 *>(address) + sizeof(DataSettings));

        if (ds->enableA)
        {
            ds->dataA = addrData;
            addrData += ds->SizeChannel();
        }

        if (ds->enableB)
        {
            ds->dataB = addrData;
        }

        WriteToRAM(address, ds, sizeof(DataSettings));
    }

    uint Address() const
    {
        return reinterpret_cast<uint>(this);
    };
    /// Возвращает адрес первого следующего за пакетом байта
    uint End() const
    {
        return Address() + Size();
    }
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
        uint address = Address() + sizeof(Packet);

        return reinterpret_cast<DataSettings *>(address);
    }
};


void RAM::Init()
{
    oldest = reinterpret_cast<Packet *>(BEGIN);
    newest = nullptr;
}


void RAM::PrepareForNewData(DataSettings *ds)
{
    ds->id = NumberDatas() ? Read()->id + 1 : 0;

    uint address = AllocateMemoryForPacket(ds);         // Находим адрес для записи нового пакета

    if (newest)
    {
        newest->addrNewest = address;                   // Указываем его в качестве адреса следующего пакета для предыдущего
    }

    newest = reinterpret_cast<Packet *>(address);       // Устанавилваем этот адрес в качестве новейшего пакета

    newest->Prepare(ds);                                   // И упаковываем данные
}


DataSettings *RAM::Read(uint numFromEnd)
{
    uint number = NumberDatas();
    
    if (numFromEnd + 1 > number)
    {
        return nullptr;
    }

    uint counter = number - 1 - numFromEnd;

    Packet *packet = oldest;

    while (counter > 0)
    {
        packet = reinterpret_cast<Packet *>(packet->addrNewest);
        counter--;
    }

    return packet->GetDataSettings();
}


void RAM::RemoveNewest()
{
    uint numDatas = RAM::NumberDatas();

    if(numDatas == 1)
    {
        newest = nullptr;
        oldest->addrNewest = 0U;
    }
    else if(numDatas > 1)
    {
        uint counter = numDatas - 1;

        Packet *packet = oldest;

        while(counter > 1)
        {
            packet = reinterpret_cast<Packet *>(packet->addrNewest);
            counter++;
        }

        newest = reinterpret_cast<Packet *>(packet->addrNewest);
        newest->addrNewest = 0U;
    }
}


uint RAM::NumberDatas()
{
    if (newest == nullptr)
    {
        return 0;
    }

    if (oldest == nullptr)
    {
        return 1;
    }

    uint result = 0;

    Packet *packet = oldest;

    while (packet != nullptr)
    {
        result++;
        packet = reinterpret_cast<Packet *>(packet->addrNewest);
    }

    return result;
}

static uint AllocateMemoryForPacket(const DataSettings *ds)
{
    if (newest == nullptr)                                                  // Ещё нет ни одной записи
    {
        return BEGIN;
    }

    uint addrFirst = newest->End();                                         // По этому адресу должна быть следующая запись
    uint addrLast = addrFirst + Packet::NeedMemoryForPacedData(ds);         // А это последний байт следующей записи

    if (addrLast > END)
    {
        addrFirst = BEGIN;
        addrLast = addrFirst + Packet::NeedMemoryForPacedData(ds);
    }
                                                                            
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
            if (oldest->Address() == BEGIN)
            {
                break;
            }
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


FrameP2P *RAM::GetFrameP2P()
{
    return nullptr;
}


void RAM::PrepareNewFrameP2P()
{

}
