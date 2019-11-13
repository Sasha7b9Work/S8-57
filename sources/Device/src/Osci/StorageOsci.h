#pragma once
#include "Data/DataSettings.h"


class Buffer;
class Reader;
class DataOsciP2P;
class Storage;

class DataOsci
{
    friend class DataOsciP2P;
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
    /// Указатель на данные первого канала
    uint8 *dataA;
    /// Указатель на данные второго канала
    uint8 *dataB;
    /// Настройки сигналов
    DataSettings settings;
protected:
    /// Указатель на предыдущие сохранённые данные
    DataOsci *prev;
    /// Указатель на следующие сохранённые данные
    DataOsci *next;
    /// Порядковый номер данных
    uint num;
    /// Сколько всего данных уже есть
    static uint allDatas;
};

class DataOsciP2P
{
    friend class Storage;
    friend class StructDataP2P;
    friend class ::Reader;
public:
    /// time - время, когда были считаны точки
    void AddPoints(BitSet16 pointA, BitSet16 pointB);
    const uint8 *DataA();
    const uint8 *DataB();
    const DataSettings *Settings();
    /// Заполнить buffer данными для вывода на экран
    void FillBufferForDraw(Chan::E ch, Buffer *buffer);

    void Create();
    /// Время начала съёма информации. Первая точка будет поставлена через время, соответствующее TBase
    uint timeStart;

    DataOsci data;
private:
    /// Время, в которое должна прийити точка numPoint
    float TimePointMS(uint numPoint) const;
    /// Возвращает true, если считано меньше точек, чем требуется для данного времени
    bool NeedAdditionPoints(uint timeMS) const;
    /// Возвращает fromEnd-й байт с конца данных
    uint8 ByteFromEnd(Chan::E ch, uint fromEnd);
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
    uint pointerToByte;
};
