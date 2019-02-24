#pragma once
#include "Data/DataSettings.h"


namespace FPGA
{
    class DataAccessor;
}

class Buffer;

class Reader;

namespace Osci
{
    class DataAccessor;
    class DataP2P;
    class Storage;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Data
    {
    friend class DataAccessor;
    friend class FPGA::DataAccessor;
    friend class DataP2P;
    friend class HeapWorker;
    public:
        void Create();
        const uint8 *DataA();
        const uint8 *DataB();
        const uint8 *GetData(Chan::E ch);
        const DataSettings *Settings();
        uint Number() const { return num; };
        /// Возвращает место, занимаемое структурой вместе с данными
        uint FullSize() const;
    protected:
        /// Указатель на данные первого канала
        uint8 *dataA;
        /// Указатель на данные второго канала
        uint8 *dataB;
        /// Указатель на предыдущие сохранённые данные
        Data *prev;
        /// Указатель на следующие сохранённые данные
        Data *next;
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
    friend class DataAccessor;
    friend class Storage;
    friend class StructDataP2P;
    friend class ::Reader;
    public:
        /// time - время, когда были считаны точки
        void AddPoints(uint timeMS, BitSet16 pointA, BitSet16 pointB);
        const uint8 *DataA();
        const uint8 *DataB();
        const DataSettings *Settings();
        /// Заполнить buffer данными для вывода на экран
        void FillBufferForDraw(Chan::E ch, Buffer *buffer);
    private:
        void Create();
        /// Время, в которое должна прийити точка numPoint
        float TimePointMS(uint numPoint) const;
        /// Возвращает true, если считано меньше точек, чем требуется для данного времени
        bool NeedAdditionPoints(uint timeMS) const;
        /// Возвращает fromEnd-й байт с конца данных
        uint8 ByteFromEnd(Chan::E ch, int fromEnd);
        /// Возвращает количество прочитанных байт
        uint ReadingBytes() const;
        /// Заполнить буфер для отрисовки при включенном детекторе
        void FillBufferForPeakDetEnabled(Chan::E ch, Buffer *buffer);
        /// Заполнить буфер для отрисовки при выключенном детекторе
        void FillBufferForPeakDetDisabled(Chan::E ch, Buffer *buffer);
        /// Подготовить буфер к заполнению данными
        void PrepareBuffer(Buffer *buffer, uint size);
        /// Количество считанных точек
        uint readingPoints;
        /// Указатель на положение байта, который будет записан следующим
        int pointerToByte;
        /// Время начала съёма информации. Первая точка будет поставлена через время, соответствующее TBase
        uint timeStart;

        Data data;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class Storage
    {
    public:
        /// Возвращает указатель на данные
        static Data *GetData(int fromEnd = 0);
        /// Очистка. Выполняется после переключения длины памяти
        static void Clear();
        /// Подготавливает место в хранилище для новых данных. Возвращает указатель на структуру, которую можно заполнять данными
        static Data *PrepareForNewData();
        /// Подготавливает новый фрейм для хранения данных поточечного вывода
        static void PrepareNewFrameP2P();
        /// Возвращает фрейм поточечного вывода
        static DataP2P *GetFrameP2P();

        static int NumElementsInStorage();
    };
}
