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
static bool TestIDN();
// *RST
static const char *FuncReset(const char *);
static bool TestReset();
// :HELP
static const char *FuncHelp(const char *);
static bool TestHelp();
// :TEST
static const char *FuncTest(const char *);
static bool TestTest();
static void Process(const StructSCPI strct[], String message);


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",     FuncIDN,        TestIDN,   "ID request"),
    SCPI_LEAF("*RST",      FuncReset,      TestReset, "Reset settings to default values"),
    SCPI_LEAF(":HELP",     FuncHelp,       TestHelp,  "Output of this help"),
    SCPI_LEAF(":TEST",     FuncTest,       TestTest,  "Run all tests"),
    SCPI_NODE(":CHANNEL",  SCPI::channels, ""),
    SCPI_NODE(":DISPLAY",  SCPI::display,  ""),
    SCPI_NODE(":KEY",      SCPI::key,      ""),
    SCPI_NODE(":TIMEBASE", SCPI::tBase,    ""),
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


static const char *FuncHelp(const char *buffer)
{
    SCPI_PROLOG(buffer);
    
    String message;

    Process(SCPI::head, message);

    SCPI_EPILOG(buffer);
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
    return true;
}


static bool TestReset()
{
    return true;
}


static bool TestHelp()
{
    return true;
}


static bool TestTest()
{
    return true;
}


static void Process(const StructSCPI strct[], String msg)
{
    while(!strct->IsEmpty())
    {
        if(strct->IsNode())
        {
            String message(msg);
            message.Append(strct->key);
            Process(strct->strct, message);
        }
        else if(strct->IsLeaf())
        {
            String message(msg);
            message.Append(strct->key);
            message.Append("   :   ");
            message.Append(strct->hint);
            SCPI::SendAnswer(message.c_str());
        }
        else
        {
            LOG_ERROR("—юда мы не можем попасть");
        }

        strct++;
    }
}