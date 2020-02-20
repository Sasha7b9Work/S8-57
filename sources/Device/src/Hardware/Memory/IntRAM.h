#pragma once
#include "Settings/ParametersOsci.h"


class IntRAM
{
public:
    // Указатель на 16k байт памяти, предназначенной для работы усреднителя
    static uint16 *Averager16k(Chan::E ch);
    // Память для чтения канала в режиме рандомизатора
    static uint8 *DataRand(Chan::E ch);
};
