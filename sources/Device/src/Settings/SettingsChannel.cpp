#include "defines.h"
#include "Settings.h"

#include "FPGA/FPGA.h"


using namespace Osci::Settings;
using namespace Osci::Settings::Memory;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Массив структур описаний масштабов по напряжению.
static const struct RangeStruct
{
    pString nameRU;     // Название диапазона в текстовом виде, пригодном для вывода на экран.
    pString nameEN;
    RangeStruct(pString nRU, pString nEN) : nameRU(nRU), nameEN(nEN) {};
}
ranges[Range::Size][2] =
{
    {RangeStruct("2\x10мВ",  "2\x10мV"),  RangeStruct("20\x10мВ", "20\x10мV")},
    {RangeStruct("5\x10мВ",  "5\x10mV"),  RangeStruct("50\x10мВ", "50\x10mV")},
    {RangeStruct("10\x10мВ", "10\x10mV"), RangeStruct("0.1\x10В", "0.1\x10V")},
    {RangeStruct("20\x10мВ", "20\x10mV"), RangeStruct("0.2\x10В", "0.2\x10V")},
    {RangeStruct("50\x10мВ", "50\x10mV"), RangeStruct("0.5\x10В", "0.5\x10V")},
    {RangeStruct("0.1\x10В", "0.1\x10V"), RangeStruct("1\x10В",   "1\x10V")},
    {RangeStruct("0.2\x10В", "0.2\x10V"), RangeStruct("2\x10В",   "2\x10V")},
    {RangeStruct("0.5\x10В", "0.5\x10V"), RangeStruct("5\x10В",   "5\x10V")},
    {RangeStruct("1\x10В",   "1\x10V"),   RangeStruct("10\x10В",  "10\x10V")},
    {RangeStruct("2\x10В",   "2\x10V"),   RangeStruct("20\x10В",  "20\x10V")},
    {RangeStruct("5\x10В",   "5\x10V"),   RangeStruct("50\x10В",  "50\x10V")},
    {RangeStruct("10\x10В",  "10\x10V"),  RangeStruct("100\x10В", "100\x10V")},
    {RangeStruct("20\x10В",  "20\x10V"),  RangeStruct("200\x10В", "200\x10V")}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Range::ToString(int8 divider)
{
    return (LANG == Language::RU) ? ranges[value][divider].nameRU : ranges[value][divider].nameEN;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Chan::PointsInChannel() const
{
    DEF_STRUCT(StructNumPoints, int) numPoints[ENumPointsFPGA::Size] =
    {
        512,
        1024,
        2048,
        4096,
        8192
    };

    return numPoints[(int)FPGA_ENUM_POINTS].val;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Chan::RequestBytes(DataSettings *) const
{
    return FPGA::MAX_NUM_POINTS;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Chan::Name() const
{
    static pString names[Chan::Size][2] =
    {
        "Канал 1", "Channel 1",
        "Канал 2", "Channel 2"
    };

    return names[value][LANG];
}
