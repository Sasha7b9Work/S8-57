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
    class DataP2P;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Data
    {
    friend class DataAccessor;
    friend class FPGA::DataAccessor;
    friend class DataP2P;
    public:
        void Create();
        const uint8 *DataA();
        const uint8 *DataB();
        const DataSettings *Settings();
    protected:
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
        void Create();
        void AddPoints(BitSet16 dataA, BitSet16 dataB);
        /// Время между точками
        uint TimePoint(uint numPoint) const;
        /// Количество считанных точек
        uint readingPoints;
        /// Указатель на положение точки, которая будет считываться следующей
        uint pointer;
        /// Время начала съёма информации. Первая точка будет поставлена через время, соответствующее TBase
        uint timeStart;

        Data data;
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
    public:
        /// Подготавливает новый фрейм для хранения данных поточечного вывода
        static void PrepareNewFrameP2P();
        /// Возвращает фрейм поточечного вывода
        static DataP2P *GetFrameP2P();
    };
}
