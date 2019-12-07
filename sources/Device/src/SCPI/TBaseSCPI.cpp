#include "defines.h"
#include "SCPI/TBaseSCPI.h"
#include "Settings/SettingsOsci.h"


// :TIME:SCALE:
static const char *FuncScale(const char *);
static bool TestScale();
static void HintScale(String *);


static const char *const tBaseNames[] =
{
    " 2NS",
    " 5NS",
    " 10NS",
    " 20NS",
    " 50NS",
    " 100NS",
    " 200NS",
    " 500NS",
    " 1US",
    " 2US",
    " 5US",
    " 10US",
    " 20US",
    " 50US",
    " 100US",
    " 200US",
    " 500US",
    " 1MS",
    " 2MS",
    " 5MS",
    " 10MS",
    " 20MS",
    " 50MS",
    " 100MS",
    " 200MS",
    " 500MS",
    " 1S",
    " 2S",
    " 5S",
    " 10S",
    ""
};


const StructSCPI SCPI::tBase[] =
{
    SCPI_LEAF(":SCALE", FuncScale, TestScale, "Horizontal zoom control", HintScale),
    SCPI_EMPTY()
};


static const char *FuncScale(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(tBaseNames[TBase()]));

    SCPI_PROCESS_ARRAY(tBaseNames, TBase::Load(static_cast<TBase::E>(i)));
}


static bool TestScale()
{
   return true;
}


static void HintScale(String *message)
{
    FUNC_HINT(message, tBaseNames)
}
