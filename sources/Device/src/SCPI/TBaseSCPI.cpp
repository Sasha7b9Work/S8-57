#include "defines.h"
#include "Osci/ParametersOsci.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"


namespace S_TBASE
{
    // :TIMEBASE:MODE
    static pCHAR FuncMode(pCHAR);
    static bool TestMode();
    static void HintMode(String *);

    // :TIMEBASE:OFFSETBASE
    static pCHAR FuncOffsetBase(pCHAR);
    static bool TestOffsetBase();
    static void HintOffsetBase(String *);
    static void AnswerOffsetBase();

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

    static void SetSampleType(int);
}


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
    SCPI_LEAF(":MODE",       S_TBASE::FuncMode,       S_TBASE::TestMode,       "",                        S_TBASE::HintMode),
    SCPI_LEAF(":PEAKDET",    S_TBASE::FuncPeakDet,    S_TBASE::TestPeakDet,    "Peak detector control",   S_TBASE::HintPeakDet),
    SCPI_LEAF(":OFFSETBASE", S_TBASE::FuncOffsetBase, S_TBASE::TestOffsetBase, "",                        S_TBASE::HintOffsetBase),
    SCPI_LEAF(":SCALE",      S_TBASE::FuncScale,      S_TBASE::TestScale,      "Horizontal zoom control", S_TBASE::HintScale),
    SCPI_LEAF(":TPOS",       S_TBASE::FuncTPos,       S_TBASE::TestTPos,       "Snap sync to screen",     S_TBASE::HintTPos),
    SCPI_EMPTY()
};


static pString modes[] =
{
    " EQUAL",
    " REAL",
    ""
};

void S_TBASE::SetSampleType(int i)
{
    PageTime::SetSampleTime(static_cast<SampleType::E>(i));
}

pCHAR S_TBASE::FuncMode(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(modes[S_RAND_SAMPLE_TYPE]));

    SCPI_PROCESS_ARRAY(modes, SetSampleType(i));
}


void S_TBASE::AnswerOffsetBase()
{
    String answer("%d", set.time._shift);
    SCPI::SendAnswer(answer.c_str());
}

pCHAR S_TBASE::FuncOffsetBase(pCHAR buffer) //-V2506
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


pCHAR S_TBASE::FuncPeakDet(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(peakdets[S_PEAK_DET]));

    SCPI_PROCESS_ARRAY(peakdets, PeakDetMode::Set(static_cast<PeakDetMode::E>(i)));
}


pCHAR S_TBASE::FuncScale(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(tBaseNames[S_TIME_BASE]));

    SCPI_PROCESS_ARRAY(tBaseNames, TBase::Set(static_cast<TBase::E>(i)));
}


pCHAR S_TBASE::FuncTPos(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(tposes[S_TPOS]));

    SCPI_PROCESS_ARRAY(tposes, TPos::Set(static_cast<TPos::E>(i)));
}


bool S_TBASE::TestMode()
{
    return true;
}


bool S_TBASE::TestOffsetBase()
{
    return false;
}


bool S_TBASE::TestPeakDet()
{
    return true;
}


bool S_TBASE::TestScale()
{
   return true;
}


bool S_TBASE::TestTPos()
{
    return true;
}


void S_TBASE::HintMode(String *message)
{
    SCPI::ProcessHint(message, modes);
}


void S_TBASE::HintOffsetBase(String *)
{

}


void S_TBASE::HintPeakDet(String *message)
{
    SCPI::ProcessHint(message, peakdets);
}


void S_TBASE::HintScale(String *message)
{
    SCPI::ProcessHint(message, tBaseNames);
}


void S_TBASE::HintTPos(String *message)
{
    SCPI::ProcessHint(message, tposes);
}
