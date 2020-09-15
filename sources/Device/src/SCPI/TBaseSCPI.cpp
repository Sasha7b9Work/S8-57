#include "defines.h"
#include "Osci/ParametersOsci.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"


// :TIMEBASE:MODE
static pCHAR FuncMode(pCHAR);
static bool TestMode();
static void HintMode(String *);

// :TIMEBASE:OFFSETBASE
static pCHAR FuncOffsetBase(pCHAR);
static bool TestOffsetBase();
static void HintOffsetBase(String *);

// :TIMEBASE:PEAKDET
static pCHAR FuncPeakDet(pCHAR);
static bool TestPeakDet();
static void HintPeakDet(String *);

// :TIMEBASE:SCALE:
static pCHAR FuncScale(pCHAR);
static bool TestScale();
static void HintScale(String *);

// :TIMEBASE:TPOS:
static pCHAR FuncTPos(pCHAR);
static bool TestTPos();
static void HintTPos(String *);



static pString tBaseNames[] =
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


static pString peakdets[] =
{
    " OFF",
    " ON",
    ""
};


static pString tposes[] =
{
    " LEFT",
    " CENTER",
    " RIGHT",
    ""
};


const StructSCPI SCPI::tBase[] =
{
    SCPI_LEAF(":MODE",       FuncMode,       TestMode,       "",                        HintMode),
    SCPI_LEAF(":PEAKDET",    FuncPeakDet,    TestPeakDet,    "Peak detector control",   HintPeakDet),
    SCPI_LEAF(":OFFSETBASE", FuncOffsetBase, TestOffsetBase, "",                        HintOffsetBase),
    SCPI_LEAF(":SCALE",      FuncScale,      TestScale,      "Horizontal zoom control", HintScale),
    SCPI_LEAF(":TPOS",       FuncTPos,       TestTPos,       "Snap sync to screen",     HintTPos),
    SCPI_EMPTY()
};


static pString modes[] =
{
    " EQUAL",
    " REAL",
    ""
};

static void SetSampleType(int i)
{
    PageTime::SetSampleTime(static_cast<SampleType::E>(i));
}

static pCHAR FuncMode(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(modes[S_RAND_SAMPLE_TYPE]));

    SCPI_PROCESS_ARRAY(modes, SetSampleType(i));
}


static void AnswerOffsetBase()
{
    String answer("%d", set.time._shift);
    SCPI::SendAnswer(answer.c_str());
}

static pCHAR FuncOffsetBase(pCHAR buffer)
{
    SCPI_REQUEST(AnswerOffsetBase());

    char *end_str = nullptr;

    int value = 0;

    if (SU::String2Int(buffer, &value, &end_str))
    {
        TShift::Set(value);

        return end_str + 1;
    }

    return nullptr;
}


static pCHAR FuncPeakDet(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(peakdets[S_PEAK_DET]));

    SCPI_PROCESS_ARRAY(peakdets, PeakDetMode::Set(static_cast<PeakDetMode::E>(i)));
}


static pCHAR FuncScale(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(tBaseNames[S_TIME_BASE]));

    SCPI_PROCESS_ARRAY(tBaseNames, TBase::Set(static_cast<TBase::E>(i)));
}


static pCHAR FuncTPos(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(tposes[S_TPOS]));

    SCPI_PROCESS_ARRAY(tposes, TPos::Set(static_cast<TPos::E>(i)));
}


static bool TestMode()
{
    return true;
}


static bool TestOffsetBase()
{
    return false;
}


static bool TestPeakDet()
{
    return true;
}


static bool TestScale()
{
   return true;
}


static bool TestTPos()
{
    return true;
}


static void HintMode(String *message)
{
    SCPI::ProcessHint(message, modes);
}


static void HintOffsetBase(String *)
{

}


static void HintPeakDet(String *message)
{
    SCPI::ProcessHint(message, peakdets);
}


static void HintScale(String *message)
{
    SCPI::ProcessHint(message, tBaseNames);
}


static void HintTPos(String *message)
{
    SCPI::ProcessHint(message, tposes);
}
