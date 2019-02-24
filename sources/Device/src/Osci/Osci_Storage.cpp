#include "defines.h"
#include "Osci/Osci_Storage.h"
#include "FPGA/FPGA.h"
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


using namespace Osci;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Osci
{

uint Data::allDatas = 0;

class DataAccessor
{
public:
    /// Заполнить вновь выделенную память нужными значениями
    static void FillNewData(Data *data)
    {
        uint bytesInChannel = FPGA::BytesInChannel();

        data->settings.Fill();

        uint8 *addrData = (uint8 *)data + sizeof(Data);     // Адрес начала данных

        if (SET_ENABLED_A)
        {
            data->dataA = addrData;
        }
        if (SET_ENABLED_B)
        {
            data->dataB = addrData;
            if (SET_ENABLED_A)
            {
                data->dataB += bytesInChannel;
            }
        }
    }

    /// Заполнить Data значениями
    static void FillNewData(DataP2P *data)
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Класс для работы с Heap
class HeapWorker
{
public:
    static uint8 *GetMemoryForData(uint /*size*/)
    {
        newest = (Data *)Heap::Begin();
        return (uint8 *)newest;
    }

    static uint8 *GetMemoryForDataP2P(uint /*size*/)
    {
        return (uint8 *)GetDataP2P();
    }

    static Data *GetData(int /* fromEnd */)
    {
        return (Data *)newest;
    }

    static DataP2P *GetDataP2P()
    {
        return (DataP2P *)(((uint)Heap::End() + (uint)Heap::Begin()) / 2);
    }

private:
    /// Указатель на первые хранящиеся данные (самые старые)
    static Data *oldest;
    /// Указатель на последние хранящиеся данные (самые свежие)
    static Data *newest;
};

Data *HeapWorker::oldest = nullptr;
Data *HeapWorker::newest = nullptr;

};


/// Рассчитать необходимое для запроса из Heap место
static uint CalculateNeedMemory();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Data *Storage::PrepareForNewData()
{
    if (SET_DISABLED_BOTH)
    {
        return nullptr;
    }

    uint bytesForSave = CalculateNeedMemory();

    Data *data = (Data *)HeapWorker::GetMemoryForData(bytesForSave);

    data->Create();

    DataAccessor::FillNewData(data);

    return data;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Storage::PrepareNewFrameP2P()
{
    uint bytesForSave = CalculateNeedMemory();

    DataP2P *data = (DataP2P *)HeapWorker::GetMemoryForDataP2P(bytesForSave);

    data->Create();

    DataAccessor::FillNewData(data);

    data->timeStart = TIME_MS;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint CalculateNeedMemory()
{
    uint result = sizeof(Data);

    uint bytesInChannel = FPGA::BytesInChannel();

    if (SET_ENABLED_A)
    {
        result += bytesInChannel;
    }
    if (SET_ENABLED_B)
    {
        result += bytesInChannel;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Data *Storage::GetData(int fromEnd)
{
    return HeapWorker::GetData(fromEnd);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DataP2P *Storage::GetFrameP2P()
{
    return HeapWorker::GetDataP2P();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Data::Create()
{
    num = allDatas++;
    dataA = dataB = nullptr;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataP2P::Create()
{
    data.Create();
    data.allDatas--;
    
    readingPoints = 0;

    pointerToByte = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const DataSettings *Data::Settings()
{
    return &settings;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const DataSettings *DataP2P::Settings()
{
    return &data.settings;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const uint8 *Data::DataA()
{
    return dataA;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const uint8 *Data::DataB()
{
    return dataB;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const uint8 *Data::GetData(Chan::E ch)
{
    return (ch == Chan::A) ? dataA : dataB;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const uint8 *DataP2P::DataA()
{
    return data.dataA;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const uint8 *DataP2P::DataB()
{
    return data.dataB;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float DataP2P::TimePointMS(uint numPoint) const
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataP2P::AddPoints(uint timeMS, BitSet16 a, BitSet16 b)
{
    if (SET_PEAKDET_EN)
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

    if (NeedAdditionPoints(timeMS))
    {
        AddPoints(timeMS, a, b);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool DataP2P::NeedAdditionPoints(uint timeMS) const
{
    float timePoint = TimePointMS(readingPoints);

    return (timePoint < timeMS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataP2P::FillBufferForDraw(Chan::E ch, Buffer *buffer)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataP2P::FillBufferForPeakDetDisabled(Chan::E ch, Buffer *buffer)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataP2P::FillBufferForPeakDetEnabled(Chan::E ch, Buffer *buffer)
{
    static const uint NUM_BYTES = 281 * 2;
    uint readingBytes = (uint)ReadingBytes();

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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void DataP2P::PrepareBuffer(Buffer *buffer, uint size)
{
    buffer->Realloc(size);
    std::memset(buffer->data, FPGA::VALUE::NONE, size);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 DataP2P::ByteFromEnd(Chan::E ch, int fromEnd)
{
    if (fromEnd > data.settings.SizeChannel())      // Если требуется значение, большее чем возможно сохранить
    {
        return FPGA::VALUE::NONE;
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint DataP2P::ReadingBytes() const
{
    if (PEAKDET_DISABLED(&data.settings))
    {
        return readingPoints;
    }

    return readingPoints * 2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Storage::NumElementsInStorage()
{
    return 1;
}
