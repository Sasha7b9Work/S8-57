#include "defines.h"
#include "log.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "SCPI/DisplaySCPI.h"
#include "Settings/Settings.h"
#include <cstring>


// :DISPLAY:MAPPING
static const char *FuncMapping(const char *);
static bool TestMapping();
static void HintMapping(String *);
// :DISPLAY:AVERAGES
static const char *FuncAverages(const char *);
static bool TestAverages();
static void HintAverages(String *);


static const char *const mapping[] =
{
    " LINES",
    " DOTS",
    ""
};


static const char *const averages[] =
{
    " 0",
    " 1",
    " 2",
    " 3",
    " 4",
    " 5",
    " 6",
    " 7",
    " 8",
    ""
};


const StructSCPI SCPI::display[] =
{
    SCPI_LEAF(":AVERAGES", FuncAverages, TestAverages, "Number of averages",     HintAverages),
    SCPI_LEAF(":MAPPING",  FuncMapping, TestMapping,   "Signal display control", HintMapping),
    SCPI_EMPTY()
};


static const char *FuncMapping(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(mapping[DisplayMapping()]));

    SCPI_PROCESS_ARRAY(mapping, DisplayMapping(static_cast<DisplayMapping::E>(i)));
}


static bool TestMapping()
{
    String commandLines(":DISPLAY:MAPPING LINEs%c", 0x0D);
    String commandDots(":DISPLAY:MApping dots%c", 0x0D);

    for(int i = 0; i < 10; i++)
    {
        SCPI_APPEND_STRING(commandLines); //-V814

        if(!DisplayMapping::IsLines())
        {
            SCPI_EXIT_ERROR();
        }

        SCPI_APPEND_STRING(commandDots); //-V814

        if(!DisplayMapping::IsDots())
        {
            SCPI_EXIT_ERROR();
        }
    }

    return true;
}


static void HintMapping(String *message)
{
    SCPI::ProcessHint(message, mapping);
}


static const char *FuncAverages(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(averages[ENumAverage()]));

    SCPI_PROCESS_ARRAY(averages, ENumAverage::Set(static_cast<ENumAverage::E>(i)));
}


static void HintAverages(String *message)
{
    SCPI::ProcessHint(message, averages);
}


static bool TestAverages()
{
    return false;
}
