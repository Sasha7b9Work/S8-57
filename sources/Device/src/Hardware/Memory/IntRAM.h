#pragma once
#include "Settings/SettingsOsci.h"


class IntRAM
{
public:
    // ”казатель на 16k байт пам€ти, предназначенной дл€ работы усреднител€
    static uint16 *Averager16k(Chan::E ch);

private:
    // Ќачало хранилища RAM, где хран€тс€ считанные сигналы
    static uint8 *Begin();
    //  онец хранилща RAM, где хран€тс€ считанные сигналы (указатель на последний байт за хранилищем)
    static uint8 *End();
};
