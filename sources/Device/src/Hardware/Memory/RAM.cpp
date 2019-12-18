#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Heap.h"
#include "Hardware/Timer.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/Osci.h"
#include "Utils/Math.h"
#include <cstring>
#include <cstdlib>


#define BEGIN reinterpret_cast<uint>(Heap::Begin())
#define END   reinterpret_cast<uint>(Heap::End())


int16 RAM::currentSignal = 0;
Packet *RAM::oldest = reinterpret_cast<Packet *>(BEGIN);
Packet *RAM::newest = nullptr;


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
        data.dataA = nullptr;
        data.dataB = nullptr;

        addrNewest = 0x0000000;                                                                         // Указываем, что это самый последний пакет

        uint *address = reinterpret_cast<uint *>(Address() + sizeof(Packet));                           // По этому адресу запишем DataSettings

        address = WriteToRAM(address, ds, sizeof(DataSettings));                                        // Записываем DataSettings

        if (ds->enableA)                                                                                // Записываем данные первого канала
        {
            data.dataA = reinterpret_cast<uint8 *>(address);
            address = WriteToRAM(address, ds->dataA, ds->BytesInChannel());
        }

        if (ds->dataB)                                                                                  // Записываем данные второго канала
        {
            data.dataB = reinterpret_cast<uint8 *>(address);
            WriteToRAM(address, ds->dataB, ds->BytesInChannel());
        }

        std::memcpy(reinterpret_cast<uint *>(Address() + sizeof(Packet)), &data, sizeof(DataSettings)); // Записываем скорректированные настройки
    }
    /// Подготовить пакет для сохранения данных в соответствии с настройками ds
    void Prepare(DataSettings *ds)
    {
        uint bytesInChannel = ds->BytesInChannel();

        addrNewest = 0x00000000;
        uint *address = reinterpret_cast<uint *>(Address() + sizeof(Packet));

        ds->dataA = nullptr;
        ds->dataB = nullptr;
        uint8 *addrData = reinterpret_cast<uint8 *>(reinterpret_cast<uint8 *>(address) + sizeof(DataSettings));

        if (ds->enableA)
        {
            ds->dataA = addrData;
            std::memset(addrData, VALUE::NONE, bytesInChannel);
            addrData += bytesInChannel;
        }

        if (ds->enableB)
        {
            ds->dataB = addrData;
            std::memset(addrData, VALUE::NONE, bytesInChannel);
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


DataSettings *RAM::PrepareForNewData()
{
    DataSettings *result = LastFrameExistAndP2P();

    if(result)
    {
        result->isFrameP2P = 0;
        TIME_MS_DS(result) = TIME_MS;       // Здесь записываем время считывания полного фрейма поточечного режима, чтобы знать, когда давать следующий старт в автоматическом режиме
        return result;
    }

    DataSettings ds;

    ds.Fill();

    ds.id = NumberDatas() ? Get()->id + 1 : 0;

    uint address = AllocateMemoryForPacket(&ds);         // Находим адрес для записи нового пакета

    if (newest)
    {
        newest->addrNewest = address;                   // Указываем его в качестве адреса следующего пакета для предыдущего
    }

    newest = reinterpret_cast<Packet *>(address);       // Устанавилваем этот адрес в качестве новейшего пакета

    newest->Prepare(&ds);                               // И упаковываем данные

    return Get();
}


DataSettings *RAM::LastFrameExistAndP2P()
{
    if(!Osci::InModeP2P() || NumberDatas() == 0)
    {
        return nullptr;
    }

    DataSettings *last = Get();

    if(last->IsFrameP2P() && last->EqualsCurrentSettings())
    {
        return last;
    }

    return nullptr;
}


bool RAM::LastFrameIsP2P()
{
    if(NumberDatas() == 0)
    {
        return false;
    }

    return Get()->IsFrameP2P();
}


DataSettings *RAM::Get(uint numFromEnd)
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

uint RAM::AllocateMemoryForPacket(const DataSettings *ds)
{
    if (newest == nullptr)                                                  // Ещё нет ни одной записи
    {
        return BEGIN;
    }

    uint addrFirst = newest->End();                                         // По этому адресу должна быть следующая запись
    uint addrLast = addrFirst + Packet::NeedMemoryForPacedData(ds);         // А это последний байт следующей записи

    if (addrLast > END)
    {
        addrLast = BEGIN + Packet::NeedMemoryForPacedData(ds);
        while (reinterpret_cast<uint>(oldest) < addrLast)
        {
            RemoveOldest();
        }
        return BEGIN;
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


void RAM::AllocateMemoryFromBegin(uint size)
{
    while (oldest->Address() - BEGIN < size)
    {
        RemoveOldest();
    }
}


void RAM::RemoveOldest()
{
    oldest = reinterpret_cast<Packet *>(oldest->addrNewest);
}
