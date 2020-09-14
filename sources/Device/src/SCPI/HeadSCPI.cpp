#include "defines.h"
#include "common/common_defines.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Menu/Pages/Include/PageService.h"
#include "SCPI/SCPI.h"


// *IDN?
static pCHAR FuncIDN(pCHAR);
static bool TestIDN();
static void HintIDN(String *);

// *RST
static pCHAR FuncReset(pCHAR);
static bool TestReset();
static void HintReset(String *);

// :HELP
static pCHAR FuncHelp(pCHAR);
static bool TestHelp();
static void HintHelp(String *);
static void ProcessHelp(const StructSCPI *strct, String message);

// :TEST
static pCHAR FuncTest(pCHAR);
static bool TestTest();
static void HintTest(String *);

// :CALIBRATOR:MODE
static pCHAR FuncCalibratorMode(pCHAR);
static bool TestCalibratorMode();
static void HintCalibratorMode(String *);


const StructSCPI SCPI::head[] =
{
    SCPI_LEAF("*IDN?",            FuncIDN,            TestIDN,            "ID request",                       HintIDN),
    SCPI_LEAF("*RST",             FuncReset,          TestReset,          "Reset settings to default values", HintReset),
    SCPI_LEAF(":HELP",            FuncHelp,           TestHelp,           "Output of this help",              HintHelp),
    SCPI_LEAF(":TEST",            FuncTest,           TestTest,           "Run all tests",                    HintTest),
    SCPI_LEAF(":CALIBRATOR:MODE", FuncCalibratorMode, TestCalibratorMode, "",                                 HintCalibratorMode),
    SCPI_NODE(":CHANNEL",         SCPI::channels),
    SCPI_NODE(":DISPLAY",         SCPI::display),
    SCPI_NODE(":FFT",             SCPI::fft),
    SCPI_NODE(":FREQMETER",       SCPI::freqmeter),
    SCPI_NODE(":KEY",             SCPI::key),
    SCPI_NODE(":MEASURE",         SCPI::measures),
    SCPI_NODE(":TESTER",          SCPI::tester),
    SCPI_NODE(":TIMEBASE",        SCPI::tBase),
    SCPI_NODE(":TRIG",            SCPI::trigger),
    SCPI_EMPTY()
};


static pCHAR FuncIDN(pCHAR buffer)
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer(String("MNIPI, %s, %s, %s", IDENTIVICATOR, VERSION, CRC32_DEVICE).c_str());

    SCPI_EPILOG(buffer)
}


static void HintIDN(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static pCHAR FuncReset(pCHAR buffer)
{
    SCPI_PROLOG(buffer)
        
    PageService::OnPress_ResetSettings();

    SCPI_EPILOG(buffer)
}


static void HintReset(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static pCHAR FuncHelp(pCHAR buffer)
{
    SCPI_PROLOG(buffer);
    
    String message;

    ProcessHelp(SCPI::head, message);

    SCPI_EPILOG(buffer);
}


static void HintHelp(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static pCHAR FuncTest(pCHAR buffer)
{
    static const char *const modes[2] =
    {
        " FULL",
        " FAST"
    };

    for (int i = 0; i < 2; i++)
    {
        pCHAR end = SCPI::BeginWith(buffer, modes[i]);
        if (end)
        {
            SCPI_PROLOG(end)

            PageDebug::PageTests::OnPress_Run();

            SCPI_EPILOG(end)
        }
    }

    return nullptr;
}


static pCHAR FuncCalibratorMode(pCHAR)
{
    return nullptr;
}


static bool TestCalibratorMode()
{
    return false;
}


static void HintCalibratorMode(String *)
{

}


static void HintTest(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
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


static void ProcessHelp(const StructSCPI *strct, String msg)
{
    while(!strct->IsEmpty())
    {
        if(strct->IsNode())
        {
            String message(msg);
            message.Append(strct->key);
            ProcessHelp(strct->strct, message);
        }
        else if(strct->IsLeaf())
        {
            String message(msg); //-V820
            message.Append(strct->key);
            SCPI::SendAnswer(strct->hint);
            strct->funcHint(&message);
            SCPI::SendAnswer("");
        }

        strct++;
    }
}
