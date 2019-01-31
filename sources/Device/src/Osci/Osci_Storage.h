#pragma once
#include "Data/DataSettings.h"


namespace FPGA
{
    class DataAccessor;
    class StorageAccessor;
}

class Reader;

namespace Osci
{
    class DataAccessor;
    class StorageAccess;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Data
    {
    friend class DataAccessor;
    friend class FPGA::DataAccessor;
    public:
        void Create();
        const uint8 *DataA();
        const uint8 *DataB();
        const DataSettings *Settings();
    private:
        /// Указатель на данные первого канала
        uint8 *dataA;
        /// Указатель на данные второго канала
        uint8 *dataB;
        /// Порядковый номер данных
        uint num;
        /// Сколько всего данных уже есть
        static uint allDatas;
        /// Настройки сигналов
        DataSettings settings;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class DataP2P
    {
    public:
        void AddPoints(BitSet16 dataA, BitSet16 dataB);
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Storage
    {
    friend class FPGA::StorageAccessor;
    friend class ::Reader;
    friend class StorageAccessor;
    private:
        /// Подготавливает место в хранилище для новых данных. Возвращает указатель на структуру, которую можно заполнять данными
        static Data *PrepareForNewData();
        /// Возвращает указатель на данные
        static Data *GetData(int fromEnd = 0);
        /// Подготавливает новый фрейм для хранения данных поточечного вывода
        static void PrepareNewFrameP2P();
        /// Возвращает фрейм поточечного вывода
        static DataP2P *GetFrameP2P();
    };
}
