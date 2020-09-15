#include "defines.h"
#include "Menu/Pages/Include/PageFreqMeter.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


// :FREQMETER:AVEPERIODS
static pCHAR FuncAvePeriods(pCHAR);
static void HintAvePeriods(String *);
static bool TestAvePeriods();

// :FREQMETER:LABELS
static pCHAR FuncLabels(pCHAR);
static void HintLabels(String *);
static bool TestLabels();

// :FREQMETER:MEASURE
static pCHAR FuncMeasure(pCHAR);
static void HintMeasure(String *);
static bool TestMeasure();

// :FREQMETER:MODE
static pCHAR FuncMode(pCHAR);
static void HintMode(String *);
static bool TestMode();

// :FREQMETER:TIME
static pCHAR FuncTime(pCHAR);
static void HintTime(String *);
static bool TestTime();

// :FREQMETER:VALUE
static pCHAR FuncValue(pCHAR);
static void HintValue(String *);
static bool TestValue();


const StructSCPI SCPI::freqmeter[] =
{
    SCPI_LEAF(":AVEPERIODS", FuncAvePeriods, TestAvePeriods, "", HintAvePeriods),
    SCPI_LEAF(":LABELS",     FuncLabels,     TestLabels,     "", HintLabels),
    SCPI_LEAF(":MEASURE",    FuncMeasure,    TestMeasure,    "", HintMeasure),
    SCPI_LEAF(":MODE",       FuncMode,       TestMode,       "", HintMode),
    SCPI_LEAF(":TIME",       FuncTime,       TestTime,       "", HintTime),
    SCPI_LEAF(":VALUE?",     FuncValue,      TestValue,      "", HintValue),
    SCPI_EMPTY()
};


static pCHAR FuncAvePeriods(pCHAR)
{
    return nullptr;
}


static pCHAR FuncLabels(pCHAR)
{
    return nullptr;
}


static pCHAR FuncMeasure(pCHAR)
{
    return nullptr;
}


static pCHAR FuncMode(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(S_FREQ_METER_ENABLED ? " ON" : " OFF"));

    SCPI_IF_BEGIN_WITH_THEN(" ON", S_FREQ_METER_ENABLED = 1);

    SCPI_IF_BEGIN_WITH_THEN(" OFF", S_FREQ_METER_ENABLED = 0);

    return nullptr;
}


static pCHAR FuncTime(pCHAR)
{
    return nullptr;
}


static pCHAR FuncValue(pCHAR)
{
    return nullptr;
}


static void HintAvePeriods(String *)
{

}


static void HintLabels(String *)
{

}


static void HintMeasure(String *)
{

}


static void HintMode(String *)
{

}


static void HintValue(String *)
{

}


static void HintTime(String *)
{

}


static bool TestAvePeriods()
{
    return false;
}


static bool TestLabels()
{
    return false;
}


static bool TestMeasure()
{
    return false;
}


static bool TestMode()
{
    return false;
}


static bool TestTime()
{
    return false;
}


static bool TestValue()
{
    return false;
}
