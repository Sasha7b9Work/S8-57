#include "defines.h"
#include "SCPI/ChannelsSCPI.h"
#include "Settings/SettingsOsci.h"
#include <cstdlib>
#include <cstring>


// :CHANNEL{1|2}:RANGE:
static const char *FuncRange(const char *);


static bool TestRange();


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
    SCPI_LEAF("RANGE", FuncRange, TestRange, "Vertical zoom control"),
    SCPI_EMPTY()
};


const StructSCPI SCPI::channels[] =
{
    SCPI_NODE("1:", chan, ""),
    SCPI_NODE("2:", chan, ""),
    SCPI_EMPTY()
};


static const char *FuncRange(const char *buffer)
{
    Chan::E ch = (*(buffer - 7) == '1') ? Chan::A : Chan::B;    // (buffer - 7) указывает на номер канала - 1 или 2

    SCPI_REQUEST(SCPI::SendAnswer(rangeName[Range(ch)]));

    SCPI_PROCESS_ARRAY(rangeName, Range(ch).Load(static_cast<Range::E>(i)))
}


static bool TestRange()
{
    for(int i = 0; i < 5; i++)
    {
        Range::E range = static_cast<Range::E>(std::rand() % Range::Count);
        String commandA(":channel1:range%s%c", rangeName[range], 0x0D);

        SCPI_APPEND_STRING(commandA);

        if(Range(Chan::A) != range)
        {
            SCPI_EXIT_ERROR();
        }

        range = static_cast<Range::E>(std::rand() % Range::Count);
        String commandB(":channel2:range%s%c", rangeName[range], 0x0D);

        SCPI_APPEND_STRING(commandB);

        if(Range(Chan::B) != range)
        {
            SCPI_EXIT_ERROR();
        }

    }

    return true;
}
