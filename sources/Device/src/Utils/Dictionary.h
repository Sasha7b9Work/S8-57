#pragma once
#include "Settings/Settings.h"


enum DictWord
{
    DHz,                // "Гц"
    DkHz,               // "кГц"
    DMHz,               // "МГц"
    Ds,                 // "с"
    Dns,                // "нс"
    Dus,                // "мкс"
    Dms,                // "мс"
    DBalanceChA,        // "Балансировка канала 1"
    DBalanceChB,        // "Балансировка канала 2"
    DCalibrateChA,      // "Калибровка канала 1"
    DCalibrateChB,      // "Калибровка канала 2"
    DTrigLev,           // "Ур синхр = "
    DTrig,              // "СИ"
    DNumWords
};


#define DICT(word)      (dictWords[word])


extern pString dictWords[DNumWords];
