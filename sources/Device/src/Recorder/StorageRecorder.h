// 2023/11/22 13:45:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/DeviceSettings.h"


struct Record;


struct Point16
{
    uint8 max;
    uint8 min;

    Point16() : max(0), min(255) {}

    Point16(BitSet16 p) : max(p.byte0), min(p.byte1) { }

    bool IsEmpty() const { return (min == 255) && (max == 0); };

    Point16 *Next(Record *record) const;

    void Erase() { min = 255; max = 0; }

    BitSet16 ToBitSet16() { return BitSet16(min, max); }
};


// Здесь хранятся точки, которые были пропущены во время вывода на дисплей
namespace BufferMissingPoints
{
    // Индекс "запоследней" точки
    extern int last;

    // Индекс первой точки
    extern int first;

    // Возвращает количество хранящихся точек
    inline int Size() { return last - first; };

    // Ложит две точки в буфер
    void Push(BitSet16 a, BitSet16 b);

    // Извлекает точки из буфера
    void Pop(BitSet16 *a, BitSet16 *b);

    // Здесь сохраняются пропущенные точки
    extern BitSet16 points[2][256];
};


struct PointFloat
{
    float min;
    float max;

    // Подготавливает точку к записи
    void Prepare();

    bool IsEmpty() const;

    void Add(float value);
};


// Описывает данные регистратора - цельную запись точек
struct Record
{
    PackedTime timeStart;       // Время записи первой точки
    int        numPoints;       // Число сохранённых точек
    uint       timeForPointMS;  // Расстояние между точками
    uint16     maxPoints;       // Если не равно 0, то длина записи ограничена maxPoints точками. При поступлении точки стирается первая точка.
    uint8      sources;         // Здесь иточники данных.  бит 0 - канал 1; бит 1 - канал 2; бит 2 - датчик. Именно
                                // в таком порядке расположены точки соответствующих источников в хранилище после структуры Record
    uint8      bytesOnPoint;    // Сколько байт нужно на одну точку всех источников
    uint8      offsetB;         // Смещение отсчётов канала B

    // Инициализировать структуру перед стартом записи
    void Init();

    // Добавление считаной точки
    void AddPoints(BitSet16 dataA, BitSet16 dataB);

    // Добавить точки, которые были пропущены во время рисования
    void AddMissingPoints();

    // Число точек в регистрограмме
    int NumPoints() const;

    // Возвращает размер свободной памяти, доступной для хранения новых данных
    uint FreeMemory() const;

    // Возвращает true, если запись корретна
    bool IsValid() const;

    // Возвращает адерс первого после записи (вместе с данными) байта
    uint8 *End() const;

    // Адрес начала записи
    uint8 *Begin() const;

    // Указатель на точку number канала A
    Point16 *ValueA(int number);

    // Указатель на точку number канала B
    Point16 *ValueB(int number);

    // Возвращает true, если запись содержит данные канала A
    bool ContainsChannelA() const;

    // Возвращает true, если запись содержит данные канала B
    bool ContainsChannelB() const;

    // Устанавливает максимальное количество точек для записи
    void SetMaxPoints(uint16 max) { maxPoints = max; }

private:

    // С этого адреса начинаются данные
    uint8 *BeginData();

    // Указатель на начало данных точки в позиции number
    uint8 *AddressPoints(int number);

    // пїЅпїЅпїЅпїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅ, пїЅпїЅпїЅпїЅ пїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅпїЅ
    void DeleteOldPoints();
};


namespace StorageRecorder
{
    // Инициализация после перехода в режим "РЕГИСТРАТОР"
    void Init();

    // Создаёт новую запись для хранения данных в хранилище
    Record *CreateNewRecord();

    // Возвращает указатель на текущую запись
    Record *LastRecord();

    // Возвращает количество сохранённых записей. 0 - последняя, 1 - предпоследняя. Если идёт запись, то в 0-ую заппсь добавляются новые данные
    uint NumRecords();
};
