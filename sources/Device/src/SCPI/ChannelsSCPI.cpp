#include "defines.h"
#include "SCPI/ChannelsSCPI.h"
#include "Settings/SettingsOsci.h"


// :CHANNEL1:RANGE:
static const char *FuncRangeA(const char *);
// :CHANNEL2:RANGE:
static const char *FuncRangeB(const char *);


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
    " 20V"
};


static const StructSCPI channel1[] =
{
    SCPI_LEAF("RANGE", FuncRangeA),
    SCPI_EMPTY()
};


static const StructSCPI channel2[] =
{
    SCPI_LEAF("RANGE", FuncRangeB),
    SCPI_EMPTY()
};


const StructSCPI SCPI::channels[] =
{
    SCPI_NODE("1:", channel1),
    SCPI_NODE("2:", channel2),
    SCPI_EMPTY()
};


static const char *FuncRangeA(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(rangeName[Range(Chan::A)]));

    for(int i = 0; i < Range::Count; i++)
    {
        end = SCPI::BeginWith(buffer, rangeName[i]);
        if(end)
        {
            SCPI_PROLOG(end)

            Range(Chan::A, static_cast<Range::E>(i));

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static const char *FuncRangeB(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(rangeName[Range(Chan::B)]));

    for(int i = 0; i < Range::Count; i++)
    {
        end = SCPI::BeginWith(buffer, rangeName[i]);
        if(end)
        {
            SCPI_PROLOG(end)

                Range(Chan::B, static_cast<Range::E>(i));

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}
