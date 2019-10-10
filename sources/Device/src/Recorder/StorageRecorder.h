#pragma once
#include "Data/DataSettings.h"


/// Точка на графике
struct PointRec
{
    PointRec(BitSet16 dataA, BitSet16 dataB)
    {
        data[Chan::A] = dataA;
        data[Chan::B] = dataB;
    }
    static PointRec CreateEmpty();
    bool IsEmpty();
    int Min(Chan::E ch);
    int Max(Chan::E ch);
private:
    BitSet16 data[2];
};


/// Описывает данные регистратора - цельную запись точек
struct Record
{
    Record() : start(0), numPoints(0), pointer(0) {} //-V730

    void SetDataAddress(uint16 *address);
    /// Добавление считаной точки
    void AddPoint(BitSet16 dataA, BitSet16 dataB);
    /// Число точек в регистрограмме
    uint NumPoints();
    /// Получить точку в позиции position
    PointRec GetPoint(uint position, uint maxPoints);
    /// Получить следующую точку
    PointRec NextPoint(uint maxPoints);
    /// Время записи первой точки
    PackedTime timeStart;
    /// Возвращает размер свободной памяти
    int FreeMemory();
private:
    /// Указатель на буфер данных - фактически адрес первой сохранённой точки
    PointRec *start;
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
