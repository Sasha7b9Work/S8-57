#include "defines.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


// :TRIGGER:SWEEP
static const char *FuncMode(const char *);
static bool TestMode();
static void HintMode(String *);


static const char *const modes[] =
{
    " AUTO",
    " NORMAL",
    " SINGLE",
    ""
};


const StructSCPI SCPI::trigger[] =
{
    SCPI_LEAF(":MODE", FuncMode, TestMode, "Set or query the trigger mode", HintMode),
    SCPI_EMPTY()
};


static const char *FuncMode(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(modes[S_TRIG_START_MODE]));

    SCPI_PROCESS_ARRAY(modes, TrigStartMode::Set(static_cast<TrigStartMode::E>(i)));
}


static void HintMode(String *message)
{
    SCPI::ProcessHint(message, modes);
}


static bool TestMode()
{
    return false;
}
