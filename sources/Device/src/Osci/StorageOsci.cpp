#include "defines.h"
#include "Osci/StorageOsci.h"
#include "FPGA/ContextOsci.h"
#include "Data/Heap.h"
#include "Settings/Settings.h"
#include <cstring>
#include "Hardware/Timer.h"
#include "Utils/Buffer.h"


/*
    Данные в Heap будут храниться последовательно. Каждые следующие данные хранятся за предыдущими. когда достигается конец памяти, стираются данные в начале и следующие пишутся на их место.
    Данные хранятся так:
    settings channel1 channel2
    При этом данные одного из каналов могут отсутствовать и в этом случае, если отсутствуют данные канала 1, за настройками идут сразу данные канала 2.
*/


uint DataOsci::allDatas = 0;

class DataAccessor
{
public:
    /// Заполнить вновь выделенную память нужными значениями
    static void FillNewData(DataOsci *data)
    {
        uint bytesInChannel = ContextOsci::BytesInChannel();

        data->settings.Fill();

        uint8 *addrData = reinterpret_cast<uint8 *>(data) + sizeof(DataOsci);     // Адрес начала данных

        if (set.ch[Chan::A].enabled)
        {
            data->dataA = addrData;
        }
        if (set.ch[Chan::B].enabled)
        {
            data->dataB = addrData;
            if (set.ch[Chan::A].enabled)
            {
                data->dataB += bytesInChannel;
            }
        }
    }

    /// Заполнить Data значениями
    static void FillNewData(DataOsciP2P *data)
    {
        FillNewData(&data->data);
    }

private:
    /// Сдвинуть значение указателя  на shift, если он не равен нулю
    static void ShiftPointerIfNotZero(uint8 **pointer, int shift)
    {
        if (*pointer)
        {
            *pointer += shift;
        }
    }
};



/// Класс для работы с Heap
class HeapWorker
{
public:

    /// Возвращает указатель на память количеством size, если таковая имеется в наличии
    static uint8 *Allocate(uint size)
    {
        if (oldest == nullptr)
        {
            return BeginMemory();
        }

        uint freeMemory = 0;
        
        if (newest >= oldest)           // Данные расположены нормально - новейшие по бОльшим адресам
        {
            if (reinterpret_cast<uint8 *>(oldest) == BeginMemory())
            {
                freeMemory = EndMemory() - reinterpret_cast<uint8 *>(newest) - newest->FullSize();
                if (freeMemory >= size)
                {
                    return reinterpret_cast<uint8 *>(newest) + newest->FullSize();
                }
            }
            else
            {
                freeMemory = static_cast<uint>(reinterpret_cast<uint8 *>(oldest) - reinterpret_cast<uint8 *>(BeginMemory()));
                if (freeMemory >= size)
                {
                    return reinterpret_cast<uint8 *>(BeginMemory());
                }
            }
        }
        else
        {
            freeMemory = reinterpret_cast<uint8 *>(oldest) - reinterpret_cast<uint8 *>(newest) - newest->FullSize();
            if (freeMemory >= size)
            {
                return reinterpret_cast<uint8 *>(newest) + newest->FullSize();
            }
        }

        return nullptr;
    }
    /// Удаление наистарейших данных
    static void FreeOldest()
    {
        if (oldest && (oldest->next != nullptr))
        {
            oldest = oldest->next;
            oldest->prev = nullptr;
        }
    }

    /// Возвращает адрес, следующий за последним доступным для сохранения данных
    static uint8 *EndMemory()
    {
        return reinterpret_cast<uint8 *>(GetMemoryForDataP2P());
    }

    /// Возвращает первый доступный адрес для хранения данных
    static uint8 *BeginMemory()
    {
        return reinterpret_cast<uint8 *>(Heap::Begin());
    }

    /// Возвращает количество сохранённых данных
    static int NumElementsInStorage()
    {
        int result = 0;

        DataOsci *data = oldest;

        while (data)
        {
            result++;
            data = data->next;
        }

        return result;
    }

    /// Выделить место для нового фрейма, чтобы хватило памяти для хранения данных с настройками DataSettings
    static DataOsci *GetMemoryForData(const DataSettings *ds)
    {
        if (oldest == nullptr)
        {
            newest = oldest = reinterpret_cast<DataOsci *>(BeginMemory());
            newest->prev = newest->next = oldest->next = oldest->prev = nullptr;
            return oldest;
        }

        uint needMemory = sizeof(DataOsci) + ds->NeedMemoryForData();       // Вычисляем требуемое количество памяти

        uint8 *address = Allocate(needMemory);

        while (address == nullptr)
        {
            FreeOldest();                           // Удаляем старые данные до тех пор, 
            address = Allocate(needMemory);         // пока не освободится достаточно место для хранения новых
        }

        DataOsci *data = reinterpret_cast<DataOsci *>(address);

        if (newest == oldest)                       // Сохранены только одни данные
        {
            oldest->next = data;
            newest = data;
            newest->prev = oldest;
            newest->next = nullptr;
        }
        else
        {
            newest->next = data;
            data->prev = newest;
            data->next = nullptr;
            newest = data;
        }

        return newest;
    }

    static DataOsci *GetData(int fromEnd)
    {
        DataOsci *data = newest;

        while (fromEnd > 0 && data != nullptr)
        {
            data = data->prev;
            fromEnd--;
        }

        return data;
    }

    static DataOsciP2P *GetMemoryForDataP2P()
    {
        return GetDataP2P();
    }

    static DataOsciP2P *GetDataP2P()
    {
        DataSettings ds;
        ds.Fill();
        uint size = sizeof(DataOsciP2P) + ds.SizeChannel() * 2;

        return reinterpret_cast<DataOsciP2P *>((reinterpret_cast<uint>(Heap::End()) - size));
    }

    static void Reset()
    {
        oldest = newest = nullptr;
    }

private:
    /// Указатель на первые хранящиеся данные (самые старые)
    static DataOsci *oldest;
    /// Указатель на последние хранящиеся данные (самые свежие)
    static DataOsci *newest;
};

DataOsci *HeapWorker::oldest = nullptr;
DataOsci *HeapWorker::newest = nullptr;



DataOsci *StorageOsci::PrepareForNewData()
{
    if (SET_DISABLED_BOTH)
    {
        return nullptr;
    }

    DataSettings ds;
    ds.Fill();

    DataOsci *data = HeapWorker::GetMemoryForData(&ds);

    data->Create();

    DataAccessor::FillNewData(data);

    return data;
}


void StorageOsci::PrepareNewFrameP2P()
{
    DataOsciP2P *data = HeapWorker::GetMemoryForDataP2P();

    data->Create();

    DataAccessor::FillNewData(data);

    data->timeStart = TIME_MS;
}


DataOsci *StorageOsci::GetData(int fromEnd)
{
    return HeapWorker::GetData(fromEnd);
}


DataOsciP2P *StorageOsci::GetFrameP2P()
{
    return HeapWorker::GetDataP2P();
}


void DataOsci::Create()
{
    num = allDatas++;
    dataA = dataB = nullptr;
}


void DataOsciP2P::Create()
{
    data.Create();
    data.allDatas--;
    
    readingPoints = 0;

    pointerToByte = 0;
}


const DataSettings *DataOsci::Settings()
{
    return &settings;
}


const DataSettings *DataOsciP2P::Settings()
{
    return &data.settings;
}


const uint8 *DataOsci::DataA()
{
    return dataA;
}


const uint8 *DataOsci::DataB()
{
    return dataB;
}


uint DataOsci::FullSize() const
{
    uint result = sizeof(DataOsci);

    if (ENABLED_A(&settings))
    {
        result += settings.SizeChannel();
    }
    if (ENABLED_B(&settings))
    {
        result += settings.SizeChannel();
    }

    return result;
}


const uint8 *DataOsci::GetData(Chan::E ch)
{
    return (ch == Chan::A) ? dataA : dataB;
}


const uint8 *DataOsciP2P::DataA()
{
    return data.dataA;
}


const uint8 *DataOsciP2P::DataB()
{
    return data.dataB;
}


float DataOsciP2P::TimePointMS(uint numPoint) const
{
    static const float timePoint[TBase::Size] =
    {
        /* 2 нс    */ 0,
        /* 5 нс    */ 0,
        /* 10 нс   */ 0,
        /* 20 нс   */ 0,
        /* 50 нс   */ 0,
        /* 100 нс  */ 0,
        /* 200 нс  */ 0,
        /* 500 нс  */ 0,
        /* 1 мкс   */ 0,
        /* 2 мкс   */ 0,
        /* 5 мкс   */ 0,
        /* 10 мкс  */ 0,
        /* 20 мкс  */ 0,
        /* 50 мкс  */ 0,
        /* 100 мкс */ 0.005F,
        /* 200 мкс */ 1.0e-2F,
        /* 500 мкс */ 2.0e-25F,
        /* 1 мс    */ 5.0e-2F,
        /* 2 мс    */ 1.0e-2F,
        /* 5 мс    */ 2.5e-1F,
        /* 10 мс   */ 5.0e-1F,
        /* 20 мс   */ 1.0e+0F,
        /* 50 мс   */ 2.5e+0F,         
        /* 100 мс  */ 5.0e+0F,         
        /* 200 мс  */ 1.0e+1F,
        /* 500 мс  */ 2.5e+1F, 
        /* 1 с     */ 5.0e+1F,
        /* 2 с     */ 1.0e+2F,
        /* 5 с     */ 2.5e+2F,
        /* 10 с    */ 5.0e+2F
    };
    
    float k = timePoint[TBASE(&data.settings)] * numPoint;
    
    return timeStart + k;
}


void DataOsciP2P::AddPoints(BitSet16 a, BitSet16 b)
{
    if (set.time.peakDet == PeakDetMode::Enabled)
    {
        data.dataA[pointerToByte] = a.byte0;
        data.dataB[pointerToByte] = b.byte0;

        pointerToByte++;
    }
    data.dataA[pointerToByte] = a.byte1;
    data.dataB[pointerToByte] = b.byte1;

    readingPoints++;

    pointerToByte++;

    if (pointerToByte == data.settings.SizeChannel())
    {
        pointerToByte = 0;
    }
}


bool DataOsciP2P::NeedAdditionPoints(uint timeMS) const
{
    float timePoint = TimePointMS(readingPoints);

    return (timePoint < timeMS);
}


void DataOsciP2P::FillBufferForDraw(Chan::E ch, Buffer *buffer)
{
    if (PEAKDET_DISABLED(&data.settings))
    {
        FillBufferForPeakDetDisabled(ch, buffer);
    }
    else
    {
        FillBufferForPeakDetEnabled(ch, buffer);
    }
}


void DataOsciP2P::FillBufferForPeakDetDisabled(Chan::E ch, Buffer *buffer)
{
    static const uint NUM_BYTES = 281;

    PrepareBuffer(buffer, NUM_BYTES);

    if (readingPoints <= NUM_BYTES)
    {
        std::memcpy(buffer->data, data.GetData(ch), readingPoints);
    }
    else
    {
        for (int i = 0; i < NUM_BYTES; i++)
        {
            buffer->data[NUM_BYTES - i] = ByteFromEnd(ch, i);
        }
    }
}


void DataOsciP2P::FillBufferForPeakDetEnabled(Chan::E ch, Buffer *buffer)
{
    static const uint NUM_BYTES = 281 * 2;
    uint readingBytes = ReadingBytes();

    PrepareBuffer(buffer, NUM_BYTES);

    if (readingBytes <= NUM_BYTES)
    {
        std::memcpy(buffer->data, data.GetData(ch), readingBytes);
    }
    else
    {
        for (int i = 0; i < NUM_BYTES; i++)
        {
            buffer->data[NUM_BYTES - i] = ByteFromEnd(ch, i);
        }
    }
}


void DataOsciP2P::PrepareBuffer(Buffer *buffer, uint size)
{
    buffer->Realloc(size);
    std::memset(buffer->data, VALUE::NONE, size);
}


uint8 DataOsciP2P::ByteFromEnd(Chan::E ch, int fromEnd)
{
    if (fromEnd > data.settings.SizeChannel())      // Если требуется значение, большее чем возможно сохранить
    {
        return VALUE::NONE;
    }

    int index = pointerToByte;        // index будет указывать на позицию возвращаемого значения

    while (fromEnd != 0)
    {
        fromEnd--;
        index--;
        if (index < 0)
        {
            index = data.settings.SizeChannel() - 1;
        }
    }

    return data.GetData(ch)[index];
}


uint DataOsciP2P::ReadingBytes() const
{
    if (PEAKDET_DISABLED(&data.settings))
    {
        return readingPoints;
    }

    return readingPoints * 2;
}


int StorageOsci::NumElementsInStorage()
{
    return HeapWorker::NumElementsInStorage();
}


void StorageOsci::Clear()
{
    HeapWorker::Reset();
}
