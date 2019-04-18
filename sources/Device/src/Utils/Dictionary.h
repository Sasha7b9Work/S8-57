#pragma once
#include "Settings/Settings.h"


enum DictWord
{
    DHz,                // "√ц"
    DkHz,               // "к√ц"
    DMHz,               // "ћ√ц"
    Ds,                 // "с"
    Dns,                // "нс"
    Dus,                // "мкс"
    DNumWords
};


#define DICT(word)      (dictWords[word])


extern pString dictWords[DNumWords];
