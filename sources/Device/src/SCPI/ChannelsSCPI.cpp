#include "defines.h"
#include "SCPI/ChannelsSCPI.h"
#include "Settings/SettingsOsci.h"


// :CHANNEL{1|2}:RANGE:
static const char *FuncRange(const char *);


static bool TestRange();
static bool TestChan1();
static bool TestChan2();


static const char *const rangeName[] =
{
    " 2MV",
    " 5MV",
    " 10MV",
    " 20MV",
    " 50MV",
    " 100MV",
    " 200MV",
    " 500MV",
    " 1V",
    " 2V",
    " 5V",
    " 10V",
    " 20V",
    ""
};


static const StructSCPI chan[] =
{
    SCPI_LEAF("RANGE", FuncRange, TestRange),
    SCPI_EMPTY()
};


const StructSCPI SCPI::channels[] =
{
    SCPI_NODE("1:", chan, TestChan1),
    SCPI_NODE("2:", chan, TestChan2),
    SCPI_EMPTY()
};


static const char *FuncRange(const char *buffer)
{
    Chan::E ch = (*(buffer - 7) == '1') ? Chan::A : Chan::B;    // (buffer - 7) указывает на номер канала - 1 или 2

    SCPI_REQUEST(SCPI::SendAnswer(rangeName[Range(ch)]));

    SCPI_PROCESS_ARRAY(rangeName, Range(ch, static_cast<Range::E>(i)))
}


static bool TestRange()
{
    return false;
}


static bool TestChan1()
{
    return false;
}


static bool TestChan2()
{
    return false;
}
