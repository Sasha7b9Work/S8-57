#include "defines.h"
#include "Osci/ParametersOsci.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


// :TIMEBASE:MODE
static const char *FuncMode(const char *);
static bool TestMode();
static void HintMode(String *);

// :TIMEBASE:PEAKDET
static const char *FuncPeakDet(const char *);
static bool TestPeakDet();
static void HintPeakDet(String *);

// :TIMEBASE:SCALE:
static const char *FuncScale(const char *);
static bool TestScale();
static void HintScale(String *);

// :TIMEBASE:TPOS:
static const char *FuncTPos(const char *);
static bool TestTPos();
static void HintTPos(String *);



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


static const char *const tposes[] =
{
    " LEFT",
    " CENTER",
    " RIGHT",
    ""
};


static const char *const peakdets[] =
{
    " OFF",
    " ON",
    ""
};


const StructSCPI SCPI::tBase[] =
{
    SCPI_LEAF(":PEAKDET", FuncPeakDet, TestPeakDet, "Peak detector control",   HintPeakDet),
    SCPI_LEAF(":SCALE",   FuncScale,   TestScale,   "Horizontal zoom control", HintScale),
    SCPI_LEAF(":TPOS",    FuncTPos,    TestTPos,    "Snap sync to screen",     HintTPos),
    SCPI_EMPTY()
};


static const char *FuncPeakDet(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(peakdets[S_PEAK_DET]));

    SCPI_PROCESS_ARRAY(peakdets, PeakDetMode::Set(static_cast<PeakDetMode::E>(i)));
}


static const char *FuncScale(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(tBaseNames[S_TIME_BASE]));

    SCPI_PROCESS_ARRAY(tBaseNames, TBase::Set(static_cast<TBase::E>(i)));
}


static const char *FuncTPos(const char *buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(tposes[S_TPOS]));

    SCPI_PROCESS_ARRAY(tposes, TPos::Set(static_cast<TPos::E>(i)));
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
