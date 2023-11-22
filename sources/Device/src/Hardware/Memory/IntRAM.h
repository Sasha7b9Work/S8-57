// 2023/11/22 13:39:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


namespace IntRAM
{
    // Указатель на 16k байт памяти, предназначенной для работы усреднителя
    uint16 *Averager32k(Chan::E ch);

    // Память для чтения канала в режиме рандомизатора
    uint8 *DataRand(Chan::E ch);

    // Указатель на данные поточечного фрейма
    DataSettings *PrepareForP2P();
};
