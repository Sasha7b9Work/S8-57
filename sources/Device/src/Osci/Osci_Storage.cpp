#include "defines.h"
#include "Osci/Osci_Storage.h"
#include "FPGA/FPGA.h"
#include "Data/Heap.h"
#include "Settings/Settings.h"
#include <cstring>


/*
    ƒанные в Heap будут хранитьс€ последовательно.  аждые следующие данные хран€тс€ за предыдущими. когда достигаетс€ конец пам€ти, стираютс€ данные в начале и следующие пишутс€ на их место.
    ƒанные хран€тс€ так:
    settings channel1 channel2
    ѕри этом данные одного из каналов могут отсутствовать и в этом случае, если отсутствуют данные канала 1, за настройками идут сразу данные канала 2.
*/


using namespace Osci;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Osci
{

uint Data::allDatas = 0;

class DataAccessor
{
public:
    /// «аполнить вновь выделенную пам€ть нужными значени€ми
    static void FillNewData(Data *data)
    {
        uint bytesInChannel = FPGA::BytesInChannel();

        data->settings.Fill();

        uint8 *addrData = (uint8 *)data + sizeof(Data);     // јдрес начала данных

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

    /// «аполнить Data значени€ми
    static void FillNewData(DataP2P *data)
    {
        FillNewData(&data->data);
    }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///  ласс дл€ работы с Heap
class HeapWorker
{
public:
    static uint8 *GetMemoryForData(uint /*size*/)
    {
        return (uint8 *)GetData(0);
    }

    static uint8 *GetMemoryForDataP2P(uint /*size*/)
    {
        return (uint8 *)GetDataP2P();
    }

    static Data *GetData(int /* fromEnd */)
    {
        return (Data *)Heap::Begin();
    }

    static DataP2P *GetDataP2P()
    {
        return (DataP2P *)(((uint)Heap::End() - (uint)Heap::Begin()) / 2);
    }

private:
    /// ”казатель на первые хран€щиес€ данные (самые старые)
    static Data *first;
    /// ”казатель на последние хран€щиес€ данные (самые свежие)
    static Data *last;
};

Data *HeapWorker::first = 0;
Data *HeapWorker::last = 0;

};


/// –ассчитать необходимое дл€ запроса из Heap место
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
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint CalculateNeedMemory()
{
    uint bytesForSave = sizeof(Data);

    uint bytesInChannel = FPGA::BytesInChannel();

    if (SET_ENABLED_A)
    {
        bytesForSave += bytesInChannel;
    }
    if (SET_ENABLED_B)
    {
        bytesForSave += bytesInChannel;
    }

    return bytesForSave;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Data *Storage::GetData(int fromEnd)
{
    return HeapWorker::GetData(fromEnd);
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

    pointer = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const DataSettings *Data::Settings()
{
    return &settings;
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
void DataP2P::AddPoints(BitSet16 a, BitSet16 b)
{
    /*
    if (pointer == data.settings.PointsInChannel())
    {
        pointer = 0;
    }

    data.dataA[pointer] = a.byte1;
    data.dataB[pointer] = b.byte1;

    pointer++;
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DataP2P *Storage::GetFrameP2P()
{
    return nullptr;
}
