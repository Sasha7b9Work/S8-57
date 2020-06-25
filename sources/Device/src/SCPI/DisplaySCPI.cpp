#include "defines.h"
#include "log.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include <cstring>


// :DISPLAY:AVERAGES
static const char *FuncAverages(const char *);
static bool TestAverages();
static void HintAverages(String *);


// :DISPLAY:MAPPING
static const char *FuncMapping(const char *);
static bool TestMapping();
static void HintMapping(String *);


static const char *const averages[] =
{
    " 1",
    " 2",
    " 4",
    " 8",
    " 16",
    " 32",
    " 64",
    " 128",
    " 256",
    ""
};


static const char *const mapping[] =
{
    " LINES",
    " DOTS",
    ""
};


const StructSCPI SCPI::display[] =
{
    SCPI_LEAF(":AVERAGE",  FuncAverages, TestAverages, "Number of averages",     HintAverages),
    SCPI_LEAF(":MAPPING",  FuncMapping, TestMapping,   "Signal display control", HintMapping),
    SCPI_EMPTY()
};


static const char *FuncAverages(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(averages[S_OSCI_ENUM_AVERAGE]));

    SCPI_PROCESS_ARRAY(averages, ENumAverage::Set(static_cast<ENumAverage::E>(i)));
}


static const char *FuncMapping(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(mapping[S_DISP_MAPPING]));

    SCPI_PROCESS_ARRAY(mapping, S_DISP_MAPPING = static_cast<DisplayMapping::E>(i));
}


static void HintAverages(String *message)
{
    SCPI::ProcessHint(message, averages);
}


static void HintMapping(String *message)
{
    SCPI::ProcessHint(message, mapping);
}


static bool TestAverages()
{
    return false;
}


static bool TestMapping()
{
    String commandLines(":DISPLAY:MAPPING LINEs%c", 0x0D);
    String commandDots(":DISPLAY:MApping dots%c", 0x0D);

    for (int i = 0; i < 10; i++)
    {
        SCPI_APPEND_STRING(commandLines); //-V814

        if (!S_DISP_MAPPING_IS_LINES)
        {
            SCPI_EXIT_ERROR();
        }

        SCPI_APPEND_STRING(commandDots); //-V814

        if (!S_DISP_MAPPING_IS_DOTS)
        {
            SCPI_EXIT_ERROR();
        }
    }

    return true;
}
