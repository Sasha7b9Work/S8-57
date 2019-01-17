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
};
