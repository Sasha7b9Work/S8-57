#include "defines.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


// :TRIG:MODE
static const char *FuncMode(const char *);
static void HintMode(String *);
static bool TestMode();


// :TRIG:SOURCE
static const char *FuncSource(const char *);
static void HintSource(String *);
static bool TestSource();


static const char *const modes[] =
{
    " AUTO",
    " NORMAL",
    " SINGLE",
    ""
};


static const char *const sources[] =
{
    " 1",
    " 2",
    ""
};


const StructSCPI SCPI::trigger[] =
{
    SCPI_LEAF(":MODE",   FuncMode,   TestMode,   "Set or query the trigger mode", HintMode),
    SCPI_LEAF(":SOURCE", FuncSource, TestSource, "Source selection",              HintSource),
    SCPI_EMPTY()
};


static const char *FuncMode(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(modes[S_TRIG_START_MODE]));

    SCPI_PROCESS_ARRAY(modes, TrigStartMode::Set(static_cast<TrigStartMode::E>(i)));
}


static const char *FuncSource(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(sources[S_TRIG_SOURCE]));

    SCPI_PROCESS_ARRAY(sources, TrigSource::Set(static_cast<Chan::E>(i)));
}


static void HintMode(String *message)
{
    SCPI::ProcessHint(message, modes);
}


static void HintSource(String *message)
{
    SCPI::ProcessHint(message, sources);
}


static bool TestMode()
{
    return false;
}


static bool TestSource()
{
    return false;
}
