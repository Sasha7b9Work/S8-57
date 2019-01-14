#include "defines.h"
#include "Settings/Settings.h"


using FPGA::SET::TBase;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int TPos::InBytes()
{
    static const int m[][2][3] =
    {
        {{0, 256, 510}, {0, 512, 1022}},
        {{0, 512, 1022}, {0, 1024, 2046}},
        {{0, 1024, 2046}, {0, 2048, 4094}},
        {{0, 2048, 4094}, {0, 4096, 8190}},
        {{0, 4096, 8190}, {0, 8192, 16382}},
        {{0, 8192, 16382}, {0, 8192, 16382}},
        {{0, 16384, 32766}, {0, 16384, 32766}}
    };
    return m[(int)FPGA_ENUM_POINTS][SET_PEAKDET][value];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString TBase::ToString() const
{
    /// Структура для описания диапазона масштаба по времени.
    static const struct StructTBase
    {
        const char *name[Language::Number];    // Название диапазона в текстовом виде, пригодном для вывода на экран.
        StructTBase(pString nRU, pString nEN) 
        {
            name[Language::RU] = nRU;
            name[Language::EN] = nEN;
        };
    }
    tBases[TBase::Number] =
    {
        StructTBase("2\x10нс",     "2\x10ns"),
        StructTBase("5\x10нс",     "5\x10ns"),
        StructTBase("10\x10нс",    "10\x10ns"),
        StructTBase("20\x10нс",    "20\x10ns"),
        StructTBase("50\x10нс",    "50\x10ns"),
        StructTBase("0.1\x10мкс",  "0.1\x10us"),
        StructTBase("0.2\x10мкс",  "0.2\x10us"),
        StructTBase("0.5\x10мкс",  "0.5\x10us"),
        StructTBase("1\x10мкс",    "1\x10us"),
        StructTBase("2\x10мкс",    "2\x10us"),
        StructTBase("5\x10мкс",    "5\x10us"),
        StructTBase("10\x10мкс",   "10\x10us"),
        StructTBase("20\x10мкс",   "20\x10us"),
        StructTBase("50\x10мкс",   "50\x10us"),
        StructTBase("0.1\x10мс",   "0.1\x10ms"),
        StructTBase("0.2\x10мс",   "0.2\x10ms"),
        StructTBase("0.5\x10мс",   "0.5\x10ms"),
        StructTBase("1\x10мс",     "1\x10ms"),
        StructTBase("2\x10мс",     "2\x10ms"),
        StructTBase("5\x10мс",     "5\x10ms"),
        StructTBase("10\x10мс",    "10\x10ms"),
        StructTBase("20\x10мс",    "20\x10ms"),
        StructTBase("50\x10мс",    "50\x10ms"),
        StructTBase("0.1\x10с",    "0.1\x10s"),
        StructTBase("0.2\x10с",    "0.2\x10s"),
        StructTBase("0.5\x10с",    "0.5\x10s"),
        StructTBase("1\x10с",      "1\x10s"),
        StructTBase("2\x10с",      "2\x10s"),
        StructTBase("5\x10с",      "5\x10s"),
        StructTBase("10\x10с",     "10\x10s")
    };
    
    return tBases[value].name[LANG];
}
