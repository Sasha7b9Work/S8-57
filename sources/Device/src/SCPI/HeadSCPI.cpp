#include "defines.h"
#include "common/common_defines.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Menu/Pages/Include/PageService.h"
#include "Osci/Osci.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


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

// :MEMORY:LENGTH
static pCHAR FuncMemoryLength(pCHAR);
static bool TestMemoryLength();
static void HintMemoryLength(String *);

// :MEMORY:SAVE
static pCHAR FuncMemorySave(pCHAR);
static bool TestMemorySave();
static void HintMemorySave(String *);

// :RUN
static pCHAR FuncRun(pCHAR);
static bool TestRun();
static void HintRun(String *);

// :STOP
static pCHAR FuncStop(pCHAR);
static bool TestStop();
static void HintStop(String *);

// :AUTOSEARCH
static pCHAR FuncAutoSearch(pCHAR);
static bool TestAutoSearch();
static void HintAutoSearch(String *);


const StructSCPI SCPI::head[] =
{
    SCPI_NODE(":TESTER",          SCPI::tester),
    SCPI_LEAF("*IDN?",            FuncIDN,            TestIDN,            "ID request",                       HintIDN),
    SCPI_LEAF("*RST",             FuncReset,          TestReset,          "Reset settings to default values", HintReset),
    SCPI_LEAF(":HELP",            FuncHelp,           TestHelp,           "Output of this help",              HintHelp),
    SCPI_LEAF(":TEST",            FuncTest,           TestTest,           "Run all tests",                    HintTest),
    SCPI_LEAF(":CALIBRATOR:MODE", FuncCalibratorMode, TestCalibratorMode, "",                                 HintCalibratorMode),
    SCPI_LEAF(":MEMORY:LENGTH",   FuncMemoryLength,   TestMemoryLength,   "",                                 HintMemoryLength),
    SCPI_LEAF(":MEMORY:SAVE",     FuncMemorySave,     TestMemorySave,     "",                                 HintMemorySave),
    SCPI_LEAF(":RUN",             FuncRun,            TestRun,            "",                                 HintRun),
    SCPI_LEAF(":STOP",            FuncStop,           TestStop,           "",                                 HintStop),
    SCPI_LEAF(":AUTOSEARCH",      FuncAutoSearch,     TestAutoSearch,     "",                                 HintAutoSearch),
    SCPI_NODE(":CHANNEL",         SCPI::channels),
    SCPI_NODE(":DISPLAY",         SCPI::display),
    SCPI_NODE(":FFT",             SCPI::fft),
    SCPI_NODE(":FREQMETER",       SCPI::freqmeter),
    SCPI_NODE(":KEY",             SCPI::key),
    SCPI_NODE(":MEASURE",         SCPI::measures),
    SCPI_NODE(":MULTIMETER",      SCPI::multimeter),
    SCPI_NODE(":TIMEBASE",        SCPI::tBase),
    SCPI_NODE(":TRIG",            SCPI::trigger),
    SCPI_EMPTY()
};


static pCHAR FuncIDN(pCHAR buffer) //-V2506
{
    SCPI_PROLOG(buffer)

    SCPI::SendAnswer(String("MNIPI, %s, %s, %s", IDENTIVICATOR, VERSION, CRC32_DEVICE).c_str());

    SCPI_EPILOG(buffer)
}


static void HintIDN(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static pCHAR FuncReset(pCHAR buffer) //-V2506
{
    SCPI_PROLOG(buffer)
        
    PageService::OnPress_ResetSettings();

    SCPI_EPILOG(buffer)
}


static void HintReset(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static pCHAR FuncHelp(pCHAR buffer) //-V2506
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


static pCHAR FuncTest(pCHAR buffer) //-V2506
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


static pCHAR FuncAutoSearch(pCHAR buffer)
{
    Osci::RunAutoSearch();
    return buffer;
}


static pString calibratorMode[] =
{
    " AC",
    " DC",
    ""
};

static void SetCalibratorMode(int i)
{
    S_SERV_CALIBRATOR_MODE = static_cast<Calibrator::Mode::E>(i);
    
    FPGA::LoadCalibratorMode();
}

static pCHAR FuncCalibratorMode(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(calibratorMode[S_SERV_CALIBRATOR_MODE]));
    
    SCPI_PROCESS_ARRAY(calibratorMode, SetCalibratorMode(i));
}


static pString length[] =
{
    " 512",
    " 1K",
    " 2K",
    " 4K",
    " 8K",
    ""
};

static pCHAR FuncMemoryLength(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(length[set.mem._enumPoints]));

    SCPI_PROCESS_ARRAY(length, set.mem._enumPoints = static_cast<ENumPointsFPGA::E>(i));
}


static pCHAR FuncMemorySave(pCHAR buffer) //-V2506
{
    SCPI_PROLOG(buffer);

    if (FDrive::IsConnected())
    {
        FDrive::SaveScreen();
    }
    else
    {
        SCPI::SendAnswer("Error save screen. Connect flash-drive.");
    }

    SCPI_EPILOG(buffer);
}


static pCHAR FuncRun(pCHAR buffer) //-V2506
{
    SCPI_PROLOG(buffer);

    Osci::Start(true);

    SCPI_EPILOG(buffer);
}


static pCHAR FuncStop(pCHAR buffer) //-V2506
{
    SCPI_PROLOG(buffer);

    Osci::Stop();

    SCPI_EPILOG(buffer);
}


static void HintAutoSearch(String *)
{

}


static void HintCalibratorMode(String *)
{

}


static void HintMemoryLength(String *)
{

}


static void HintMemorySave(String *)
{

}


static void HintRun(String *)
{

}


static void HintStop(String *)
{

}


static void HintTest(String *message) //-V2009 //-V2558
{
    SCPI::SendAnswer(message->c_str());
}


static bool TestAutoSearch()
{
    return false;
}


static bool TestCalibratorMode()
{
    return false;
}


static bool TestMemoryLength()
{
    return false;
}


static bool TestMemorySave()
{
    return false;
}


static bool TestRun()
{
    return false;
}


static bool TestStop()
{
    return false;
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
        else if(strct->IsLeaf()) //-V2516
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
