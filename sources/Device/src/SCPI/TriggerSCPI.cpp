#include "defines.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


// :TRIG:INPUT
static const char *FuncInput(const char *);
static void HintInput(String *);
static bool TestInput();

// :TRIG:MODE
static const char *FuncMode(const char *);
static void HintMode(String *);
static bool TestMode();

// :TRIG:POLARITY
static const char *FuncPolarity(const char *);
static void HintPolarity(String *);
static bool TestPolarity();

// :TRIG:SOURCE
static const char *FuncSource(const char *);
static void HintSource(String *);
static bool TestSource();


static const char *const inputs[] =
{
    " FULL",
    " HF",
    " LF",
    ""
};


static const char *const modes[] =
{
    " AUTO",
    " NORMAL",
    " SINGLE",
    ""
};


static const char *const polarities[] =
{
    " RISE",
    " FALL",
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
    SCPI_LEAF(":INPUT",    FuncInput,    TestInput,    "Filter selection by synchronization", HintInput),
    SCPI_LEAF(":MODE",     FuncMode,     TestMode,     "Set or query the trigger mode",       HintMode),
    SCPI_LEAF(":POLARITY", FuncPolarity, TestPolarity, "Sync polarity selection",             HintPolarity),
    SCPI_LEAF(":SOURCE",   FuncSource,   TestSource,   "Source selection",                    HintSource),
    SCPI_EMPTY()
};


static const char *FuncInput(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(inputs[S_TRIG_INPUT]));

    SCPI_PROCESS_ARRAY(inputs, TrigInput::Set(static_cast<TrigInput::E>(i)));
}


static const char *FuncMode(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(modes[S_TRIG_START_MODE]));

    SCPI_PROCESS_ARRAY(modes, TrigStartMode::Set(static_cast<TrigStartMode::E>(i)));
}


static const char *FuncPolarity(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(polarities[S_TRIG_POLARITY]));

    SCPI_PROCESS_ARRAY(polarities, TrigPolarity::Set(static_cast<TrigPolarity::E>(i)));
}


static const char *FuncSource(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(sources[S_TRIG_SOURCE]));

    SCPI_PROCESS_ARRAY(sources, TrigSource::Set(static_cast<Chan::E>(i)));
}


static void HintInput(String *message)
{
    SCPI::ProcessHint(message, inputs);
}


static void HintMode(String *message)
{
    SCPI::ProcessHint(message, modes);
}


static void HintPolarity(String *message)
{
    SCPI::ProcessHint(message, polarities);
}


static void HintSource(String *message)
{
    SCPI::ProcessHint(message, sources);
}


static bool TestInput()
{
    return false;
}


static bool TestMode()
{
    return false;
}


static bool TestPolarity()
{
    return false;
}


static bool TestSource()
{
    return false;
}
