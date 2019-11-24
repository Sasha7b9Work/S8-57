#include "defines.h"
#include "SCPI/ChannelsSCPI.h"
#include "Settings/SettingsOsci.h"


// :CHANNEL{1|2}:RANGE:
static const char *FuncRange(const char *);


static const char *const rangeName[Range::Count] =
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
    " 20V"
};


static const StructSCPI chan[] =
{
    SCPI_LEAF("RANGE", FuncRange),
    SCPI_EMPTY()
};


const StructSCPI SCPI::channels[] =
{
    SCPI_NODE("1:", chan),
    SCPI_NODE("2:", chan),
    SCPI_EMPTY()
};


static const char *FuncRange(const char *buffer)
{
    Chan::E ch = (*(buffer - 7) == '1') ? Chan::A : Chan::B;    // (buffer - 7) указывает на номер канала - 1 или 2

    SCPI_REQUEST(SCPI::SendAnswer(rangeName[Range(ch)]));

    SCPI_PROCESS_ARRAY(Range::Count, rangeName, Range(ch, static_cast<Range::E>(i)))
}
