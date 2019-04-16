#include "defines.h"
#include "Settings.h"

#include "FPGA/FPGA.h"


using namespace Osci::Settings;
using namespace Osci::Settings::Memory;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Массив структур описаний масштабов по напряжению.
static const struct RangeStruct
{
    pString name;     // Название диапазона в текстовом виде, пригодном для вывода на экран.
    RangeStruct(pString nRU) : name(nRU) {};
}
ranges[Range::Size][2] =
{
    {RangeStruct("2\x10мВ"),  RangeStruct("20\x10мВ")},
    {RangeStruct("5\x10мВ"),  RangeStruct("50\x10мВ")},
    {RangeStruct("10\x10мВ"), RangeStruct("0.1\x10В")},
    {RangeStruct("20\x10мВ"), RangeStruct("0.2\x10В")},
    {RangeStruct("50\x10мВ"), RangeStruct("0.5\x10В")},
    {RangeStruct("0.1\x10В"), RangeStruct("1\x10В")},
    {RangeStruct("0.2\x10В"), RangeStruct("2\x10В")},
    {RangeStruct("0.5\x10В"), RangeStruct("5\x10В")},
    {RangeStruct("1\x10В"),   RangeStruct("10\x10В")},
    {RangeStruct("2\x10В"),   RangeStruct("20\x10В")},
    {RangeStruct("5\x10В"),   RangeStruct("50\x10В")},
    {RangeStruct("10\x10В"),  RangeStruct("100\x10В")},
    {RangeStruct("20\x10В"),  RangeStruct("200\x10В")}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Range::ToString(int8 _divider)
{
    return ranges[value][_divider].name;
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
    static pString names[Chan::Size] =
    {
        "Канал 1",
        "Канал 2"
    };

    return names[value];
}
