#include "defines.h"
#include "SCPI/TBaseSCPI.h"
#include "Settings/SettingsOsci.h"


// :TIME:SCALE:
static const char *FuncTBaseScale(const char *);


static const char *const tBaseNames[TBase::Count] =
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
    " 10S"
};


const StructSCPI SCPI::tBase[] =
{
    SCPI_LEAF(":SCALE", FuncTBaseScale),
    SCPI_EMPTY()
};


static const char *FuncTBaseScale(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(tBaseNames[TBase()]));

    for(int i = 0; i < TBase::Count; i++)
    {
        end = SCPI::BeginWith(buffer, tBaseNames[i]);
        if(end)
        {
            SCPI_PROLOG(end)

            TBase(static_cast<TBase::E>(i));

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}
