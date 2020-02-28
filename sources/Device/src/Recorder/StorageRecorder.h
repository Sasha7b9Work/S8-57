#pragma once
#include "Osci/DeviceSettings.h"


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
    uint       numPoints;       // Число сохранённых точек
    uint8      sources;         // Здесь иточники данных.
                                // бит 0 - канал 1; бит 1 - канал 2; бит 2 - датчик. Именно в таком порядке расположены точки соответствующих источников в хранилище после структуры Record
    uint8      bytesOnPoint;    // Сколько байт нужно на одну точку всех источников

    uint8      offsetB;         // Смещение отсчётов канала B
    uint8      offsetSensor;    // Смещение отсчётов датчика

    // Инициализировать структуру перед стартом записи
    void Init();

    // Добавление считаной точки
    void AddPoints(BitSet16 dataA, BitSet16 dataB);

    // Добавление точки датчика в запись
    void AddPoint(float value);

    // Число точек в регистрограмме
    uint NumPoints() const;

    // Возвращает размер свободной памяти, доступной для хранения новых данных
    uint FreeMemory() const;

    // Возвращает true, если запись корретна
    bool IsValid() const;

    // Возвращает адерс первого следующего за записью байта
    uint8 *End() const;

    uint8 *Begin() const;

private:
    
    BitSet16 *ValueA(uint number);          // Указатель на точку number канала A
    BitSet16 *ValueB(uint number);          // Указатель на точку number канала B
    PointFloat *ValueSensor(uint number);    // Указатель на точку number датчика

    // С этого адреса начинаются данные
    uint8 *BeginData();

    // Указатель на начало данных точки в позиции number
    uint8 *AddressPoints(uint number);
};


struct StorageRecorder
{
    // Инициализация после перехода в режим "РЕГИСТРАТОР"
    static void Init();

    // Создаёт новую запись для хранения данных в хранилище
    static bool CreateNewRecord();

    // Возвращает указатель на текущую запись
    static Record *LastRecord();

    // Возвращает количество сохранённых записей. 0 - последняя, 1 - предпоследняя. Если идёт запись, то в 0-ую заппсь добавляются новые данные
    static uint NumRecords();
};
