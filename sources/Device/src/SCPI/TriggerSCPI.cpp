#include "defines.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"


// :TRIG:INPUT
static pCHAR FuncInput(pCHAR);
static void HintInput(String *);
static bool TestInput();

// :TRIG:LEVEL
static pCHAR FuncLevel(pCHAR);
static void HintLevel(String *);
static bool TestLevel();

// :TRIG:MODE
static pCHAR FuncMode(pCHAR);
static void HintMode(String *);
static bool TestMode();

// :TRIG:POLARITY
static pCHAR FuncPolarity(pCHAR);
static void HintPolarity(String *);
static bool TestPolarity();

// :TRIG:SOURCE
static pCHAR FuncSource(pCHAR);
static void HintSource(String *);
static bool TestSource();


static pString inputs[] =
{
    " FULL",
    " HF",
    " LF",
    ""
};


static pString modes[] =
{
    " AUTO",
    " NORMAL",
    " SINGLE",
    ""
};


static pString polarities[] =
{
    " RISE",
    " FALL",
    ""
};


static pString sources[] =
{
    " 1",
    " 2",
    ""
};


const StructSCPI SCPI::trigger[] =
{
    SCPI_LEAF(":INPUT",    FuncInput,    TestInput,    "Filter selection by synchronization", HintInput),
    SCPI_LEAF(":LEVEL",    FuncLevel,    TestLevel,    "",                                    HintLevel),
    SCPI_LEAF(":MODE",     FuncMode,     TestMode,     "Set or query the trigger mode",       HintMode),
    SCPI_LEAF(":POLARITY", FuncPolarity, TestPolarity, "Sync polarity selection",             HintPolarity),
    SCPI_LEAF(":SOURCE",   FuncSource,   TestSource,   "Source selection",                    HintSource),
    SCPI_EMPTY()
};


static pCHAR FuncInput(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(inputs[S_TRIG_INPUT]));

    SCPI_PROCESS_ARRAY(inputs, TrigInput::Set(static_cast<TrigInput::E>(i))); //-V2563
}


static void SendAnswerLevel()
{
    String answer("%d", S_TRIG_LEVEL_SOURCE / 2);
    SCPI::SendAnswer(answer.c_str());
}

static pCHAR FuncLevel(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SendAnswerLevel());

    char *end_str = nullptr;

    int value = 0;

    if (SU::String2Int(buffer, &value, &end_str))
    {
        TrigLevel::Set(S_TRIG_SOURCE, static_cast<int16>(value * 2));

        return end_str + 1; //-V2563
    }

    return nullptr;
}


static pCHAR FuncMode(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(modes[S_TRIG_START_MODE]));

    SCPI_PROCESS_ARRAY(modes, TrigStartMode::Set(static_cast<TrigStartMode::E>(i))); //-V2563
}


static pCHAR FuncPolarity(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(polarities[S_TRIG_POLARITY]));

    SCPI_PROCESS_ARRAY(polarities, TrigPolarity::Set(static_cast<TrigPolarity::E>(i))); //-V2563
}


static pCHAR FuncSource(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(sources[S_TRIG_SOURCE]));

    SCPI_PROCESS_ARRAY(sources, TrigSource::Set(static_cast<Chan::E>(i))); //-V2563
}


static void HintInput(String *message)
{
    SCPI::ProcessHint(message, inputs);
}


static void HintLevel(String *)
{

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


static bool TestLevel()
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
