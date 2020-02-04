#pragma once
#include "Osci/DataSettings.h"


/// Точка на графике
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


/// Описывает данные регистратора - цельную запись точек
struct Record
{
    Record() : start(nullptr), numPoints(0), pointer(0) {} //-V730

    void SetDataAddress(uint16 *addressMC);
    /// Добавление считаной точки
    void AddPoint(BitSet16 dataA, BitSet16 dataB);
    /// Число точек в регистрограмме
    uint NumPoints();
    /// Получить точку в позиции position
    Point GetPoint(uint position, uint maxPoints);
    /// Получить следующую точку
    Point NextPoint(uint maxPoints);
    /// Время записи первой точки
    PackedTime timeStart;
    /// Возвращает размер свободной памяти
    int FreeMemory();
private:
    /// Указатель на буфер данных - фактически адрес первой сохранённой точки
    Point *start;
    /// Количество сохранённых точек
    uint numPoints;
    /// Указатель на последние считаннные данные
    uint pointer;
};


struct StorageRecorder
{
    static void CreateNewRecord();

    static Record *CurrentRecord();
};
