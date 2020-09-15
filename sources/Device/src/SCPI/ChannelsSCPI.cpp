#include "defines.h"
#include "log.h"
#include "Osci/ParametersOsci.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include <cstdlib>
#include <cstring>


#define EXTRACT_CHANNEL(x) Chan::E ch = (*(buffer - (x)) == '1') ? ChanA : ChanB;    /* (buffer - 7) указывает на номер канала - 1 или 2 */

// :CHANNEL{1|2}:BALANCE
static pCHAR FuncBalance(pCHAR);
static bool TestBalance();
static void HintBalance(String *);

// :CHANNEL{1|2}:BANDWIDTH
static pCHAR FuncBandwidth(pCHAR);
static bool TestBandwidth();
static void HintBandwidth(String *);

// :CHANNEL{1|2}:COUPLING
static pCHAR FuncCoupling(pCHAR);
static bool TestCoupling();
static void HintCoupling(String *);

// :CHANNEL{1|2}:DATA
static pCHAR FuncData(pCHAR);
static bool TestData();
static void HintData(String *);

// :CHANNEL{1|2}:DISPLAY
static pCHAR FuncDisplay(pCHAR);
static bool TestDisplay();
static void HintDisplay(String *);

// :CHANNEL{1|2}:OFFSET
static pCHAR FuncOffset(pCHAR);
static bool TestOffset();
static void HintOffset(String *);

// :CHANNEL{1|2}:PROBE
static pCHAR FuncProbe(pCHAR);
static bool TestProbe();
static void HintProbe(String *);

// :CHANNEL{1|2}:SCALE
static pCHAR FuncScale(pCHAR);
static bool TestScale();
static void HintScale(String *);


static pString displays[] =
{
    " OFF",
    " ON",
    ""
};


static pString rangeName[] =
{
    " 2MV",
    " 5MV",
    " 10MV",
    " 20MV",
    " 50MV",
    " 100MV",
    " 200MV",
    " 500MV",
    " 1V",
    " 2V",
    " 5V",
    " 10V",
    " 20V",
    ""
};


static const StructSCPI chan[] =
{
    SCPI_LEAF("BALANCE",   FuncBalance,   TestBalance,   "",                             HintBalance),
    SCPI_LEAF("BANDWIDTH", FuncBandwidth, TestBandwidth, "",                             HintBandwidth),
    SCPI_LEAF("COUPLING",  FuncCoupling,  TestCoupling,  "",                             HintCoupling),
    SCPI_LEAF("DATA?",     FuncData,      TestData,      "",                             HintData),
    SCPI_LEAF("DISPLAY",   FuncDisplay,   TestDisplay,   "Turns channel display on/off", HintDisplay),
    SCPI_LEAF("OFFSET",    FuncOffset,    TestOffset,    "",                             HintOffset),
    SCPI_LEAF("PROBE",     FuncProbe,     TestProbe,     "",                             HintProbe),
    SCPI_LEAF("SCALE",     FuncScale,     TestScale,     "Vertical zoom control",        HintScale),
    SCPI_EMPTY()
};


const StructSCPI SCPI::channels[] =
{
    
    SCPI_NODE("1:", chan),
    SCPI_NODE("2:", chan),
    SCPI_EMPTY()
};


static pCHAR FuncBalance(pCHAR)
{
    return nullptr;
}


static pCHAR FuncBandwidth(pCHAR)
{
    return nullptr;
}


static pString couple[] =
{
    " DC",
    " AC",
    " GND",
    ""
};


static pCHAR FuncCoupling(pCHAR buffer)
{
    EXTRACT_CHANNEL(10);

    SCPI_REQUEST(SCPI::SendAnswer(couple[S_MODE_COUPLE(ch)]));

    SCPI_PROCESS_ARRAY(couple, ModeCouple::Set(ch, static_cast<ModeCouple::E>(i)));
}


static pCHAR FuncData(pCHAR)
{
    return nullptr;
}


static pCHAR FuncDisplay(pCHAR buffer)
{
    EXTRACT_CHANNEL(9);

    SCPI_REQUEST(SCPI::SendAnswer(displays[S_CHANNEL_ENABLED(ch)]));

    SCPI_PROCESS_ARRAY(displays, PageChannel::Enable(ch, i));
}


static pCHAR FuncOffset(pCHAR)
{
    return nullptr;
}


static pCHAR FuncProbe(pCHAR)
{
    return nullptr;
}


static pCHAR FuncScale(pCHAR buffer)
{
    EXTRACT_CHANNEL(7);

    SCPI_REQUEST(SCPI::SendAnswer(rangeName[S_RANGE(ch)]));

    SCPI_PROCESS_ARRAY(rangeName, Range::Set(ch, static_cast<Range::E>(i)))
}


static void HintBalance(String *)
{

}


static void HintBandwidth(String *)
{

}


static void HintCoupling(String *)
{

}


static void HintData(String *)
{

}


static void HintDisplay(String *message)
{
    SCPI::ProcessHint(message, displays);
}


static void HintOffset(String *)
{

}


static void HintProbe(String *)
{

}


static void HintScale(String *message)
{
    SCPI::ProcessHint(message, rangeName);
}


static bool TestBalance()
{
    return false;
}


static bool TestBandwidth()
{
    return false;
}


static bool TestCoupling()
{
    return false;
}


static bool TestData()
{
    return false;
}


static bool TestDisplay()
{
    return false;
}


static bool TestOffset()
{
    return false;
}


static bool TestProbe()
{
    return false;
}


static bool TestScale()
{
    for(int i = 0; i < 5; i++)
    {
        Range::E range = static_cast<Range::E>(std::rand() % Range::Count);
        String commandA(":channel1:range%s%c", rangeName[range], 0x0D);

        SCPI_APPEND_STRING(commandA);

        if(S_RANGE_A != range)
        {
            SCPI_EXIT_ERROR();
        }

        range = static_cast<Range::E>(std::rand() % Range::Count);
        String commandB(":channel2:range%s%c", rangeName[range], 0x0D);

        SCPI_APPEND_STRING(commandB);

        if(S_RANGE_B != range)
        {
            SCPI_EXIT_ERROR();
        }

    }

    return true;
}
