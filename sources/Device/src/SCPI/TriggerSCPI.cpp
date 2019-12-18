#include "defines.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "SCPI/TriggerSCPI.h"


// :TRIGGER:SWEEP
static const char *FuncSweep(const char *);
static bool TestSweep();
static void HintSweep(String *);


static const char *const sweep[] =
{
    " AUTO",
    " NORMAL",
    " SINGLE",
    ""
};


const StructSCPI SCPI::trigger[] =
{
    SCPI_LEAF(":SWEEP", FuncSweep, TestSweep, "Set or query the trigger mode", HintSweep),
    SCPI_EMPTY()
};


static const char *FuncSweep(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(sweep[TrigStartMode()]));

    SCPI_PROCESS_ARRAY(sweep, TrigStartMode::Set(static_cast<TrigStartMode::E>(i)));
}


static void HintSweep(String *message)
{
    SCPI::ProcessHint(message, sweep);
}


static bool TestSweep()
{
    return false;
}
