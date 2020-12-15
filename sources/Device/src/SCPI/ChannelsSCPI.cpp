#include "defines.h"
#include "log.h"
#include "Osci/Osci.h"
#include "Osci/ParametersOsci.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
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


static pCHAR FuncBalance(pCHAR buffer) //-V2506
{
    EXTRACT_CHANNEL(9);

    SCPI_PROLOG(buffer);

    Calibrator::BalanceChannel(ch, true);

    SCPI_EPILOG(buffer);
}


static pString bandwidth[] =
{
    " FULL",
    " 20MHZ",
    ""
};


static void SetBandwidth(Chan::E ch, int i)
{
    S_BANDWIDTH(ch) = static_cast<Bandwidth::E>(i);
    Bandwidth::Load(ch);
}


static pCHAR FuncBandwidth(pCHAR buffer) //-V2506
{
    EXTRACT_CHANNEL(11);

    SCPI_REQUEST(SCPI::SendAnswer(bandwidth[S_BANDWIDTH(ch)]));

    SCPI_PROCESS_ARRAY(bandwidth, SetBandwidth(ch, i));
}


static pString couple[] =
{
    " DC",
    " AC",
    " GND",
    ""
};


static pCHAR FuncCoupling(pCHAR buffer) //-V2506
{
    EXTRACT_CHANNEL(10);

    SCPI_REQUEST(SCPI::SendAnswer(couple[S_MODE_COUPLE(ch)]));

    SCPI_PROCESS_ARRAY(couple, ModeCouple::Set(ch, static_cast<ModeCouple::E>(i)));
}


static pCHAR FuncData(pCHAR buffer) //-V2506
{
    EXTRACT_CHANNEL(7);

    SCPI_PROLOG(buffer);

    SCPI::Sender::osci[ch] = true;

    SCPI_EPILOG(buffer);
}


static pCHAR FuncDisplay(pCHAR buffer) //-V2506
{
    EXTRACT_CHANNEL(9);

    SCPI_REQUEST(SCPI::SendAnswer(displays[S_CHANNEL_ENABLED(ch)]));

    SCPI_PROCESS_ARRAY(displays, PageChannel::Enable(ch, i));
}


static void SendAnswerForOffst(Chan::E ch)
{
    String answer("%d", set.ch[ch]._rShift / 2);
    SCPI::SendAnswer(answer.c_str());
}

static pCHAR FuncOffset(pCHAR buffer) //-V2506
{
    EXTRACT_CHANNEL(8);

    SCPI_REQUEST(SendAnswerForOffst(ch));

    char *end_str = nullptr;

    int value = 0;

    if (SU::String2Int(buffer, &value, &end_str))
    {
        RShift::Set(ch, static_cast<int16>(value * 2));

        return end_str + 1;
    }

    return nullptr;
}


static pString probe[] =
{
    " X1",
    " X10",
    ""
};


static pCHAR FuncProbe(pCHAR buffer) //-V2506
{
    EXTRACT_CHANNEL(7);

    SCPI_REQUEST(SCPI::SendAnswer(probe[S_DIVIDER(ch)]));

    end = SCPI::BeginWith(buffer, " X10");

    if (end)
    {
        SCPI_PROLOG(end);
        S_DIVIDER(ch) = Divider::_10;
        SCPI_EPILOG(end);
    }

    end = SCPI::BeginWith(buffer, " X1");
    if (end)
    {
        SCPI_PROLOG(end);
        S_DIVIDER(ch) = Divider::_1;
        SCPI_EPILOG(end);
    }

    return nullptr;
}


static pCHAR FuncScale(pCHAR buffer) //-V2506
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


static bool TestScale() //-V2506
{
    for(int i = 0; i < 5; i++)
    {
        Range::E range = static_cast<Range::E>(std::rand() % Range::Count);
        String commandA(":channel1:range%s%c", rangeName[range], 0x0D);

        SCPI_APPEND_STRING(commandA); //-V2513

        if(S_RANGE_A != range)
        {
            SCPI_EXIT_ERROR();
        }

        range = static_cast<Range::E>(std::rand() % Range::Count);
        String commandB(":channel2:range%s%c", rangeName[range], 0x0D);

        SCPI_APPEND_STRING(commandB); //-V2513

        if(S_RANGE_B != range)
        {
            SCPI_EXIT_ERROR();
        }

    }

    return true;
}
