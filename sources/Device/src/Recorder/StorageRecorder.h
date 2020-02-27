#pragma once
#include "Osci/DeviceSettings.h"


// Точка на графике
struct Point
{
    Point(BitSet16 dataA, BitSet16 dataB)
    {
        data[Chan::A] = dataA;
        data[Chan::B] = dataB;
    }
    static Point CreateEmpty();
    bool IsEmpty();
    int Min(Chan::E ch);
    int Max(Chan::E ch);
private:
    BitSet16 data[2];
};


// Описывает данные регистратора - цельную запись точек
struct Record
{
    // Время записи первой точки
    PackedTime timeStart;

    // Здесь иточники данных.
    // бит 0 - канал 1; бит 1 - канал 2; бит 2 - датчик. Именно в таком порядке расположены точки соответствующих источников в хранилище после структуры Record
    uint8 bitMaskSources;

    // Число сохранённых точек
    uint numPoints;

    void Init();

    // Добавление считаной точки
    void AddPoint(BitSet16 dataA, BitSet16 dataB);

    // Число точек в регистрограмме
    uint NumPoints();

    // Возвращает размер свободной памяти
    int FreeMemory();
};


struct StorageRecorder
{
    // Инициализация после перехода в режим "РЕГИСТРАТОР"
    static void Init();

    // Создаёт новую запись для хранения данных в хранилище
    static void CreateNewRecord();

    // Возвращает указатель на текущую запись
    static Record *CurrentRecord();
};
