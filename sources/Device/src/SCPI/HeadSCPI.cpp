#include "defines.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Menu/Pages/Include/PageService.h"
#include "SCPI/ChannelsSCPI.h"
#include "SCPI/DisplaySCPI.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/KeySCPI.h"
#include "SCPI/TBaseSCPI.h"


// *IDN?
static const char *FuncIDN(const char *);
// *RST
static const char *FuncReset(const char *);
// :TEST
static const char *FuncTest(const char *);


static bool TestIDN();
static bool TestReset();
static bool TestTest();


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",     FuncIDN,        TestIDN),
    SCPI_LEAF("*RST",      FuncReset,      TestReset),
    SCPI_LEAF(":TEST",     FuncTest,       TestTest),
    SCPI_NODE(":CHANNEL",  SCPI::channels),
    SCPI_NODE(":DISPLAY",  SCPI::display),
    SCPI_NODE(":KEY",      SCPI::key),
    SCPI_NODE(":TIMEBASE", SCPI::tBase),
    SCPI_EMPTY()
};


static const char *FuncIDN(const char *buffer)
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer("MNIPI, S8-57, v.1.2");

    SCPI_EPILOG(buffer)
}


static const char *FuncReset(const char *buffer)
{
    SCPI_PROLOG(buffer)
        
    PageService::OnPress_ResetSettings();

    SCPI_EPILOG(buffer)
}


static const char *FuncTest(const char *buffer)
{
    static const char *const modes[2] =
    {
        " FULL",
        " FAST"
    };

    for (int i = 0; i < 2; i++)
    {
        const char *end = SCPI::BeginWith(buffer, modes[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            PageDebug::PageTests::OnPress_Run();

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static bool TestIDN()
{
    return false;
}


static bool TestReset()
{
    return false;
}


static bool TestTest()
{
    return false;
}
