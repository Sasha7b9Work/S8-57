#pragma once
#include "Data/DataSettings.h"


/// Здесь хранятся данные 
class DataBuffer
{
public:
    static void Init();

    static uint Size();
    /// Возвращает указатель на данные, помещённые в буфер последними
    static DataSettings *Top();

private:
    /// Помещает данные, описанные в DataSettings, последними в буфер. Если места не хватает, то самые старые помещённые данные удаляются
    static void Push(DataSettings *ds);
    /// Вспомогательный класс для урощения работы с массивом DataSettings
    class Stack
    {
    public:

        static uint Size();
        /// В конструктор передаётся адрес первого элемента и количество элементов
        static void Clear();

        static void Push(DataSettings *ds);

        static DataSettings *Top();
    };
};
