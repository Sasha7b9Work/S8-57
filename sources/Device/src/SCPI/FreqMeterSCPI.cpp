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


static pString periodsReverse[] =
{
    " 1E0",
    " 1E1",
    " 1E2",
    " 1E3",
    " 1E4",
    " 1E5",
    ""
};

static void SetNumberPeriods(int i)
{
    if (S_FREQ_MODE_MEASURE_IS_PERIOD)
    {
        S_FREQ_NUMBER_PERIODS = static_cast<FreqMeter::NumberPeriods::E>(i);

        FreqMeter::LoadPeriodSettings();
    }
}

static pCHAR FuncAvePeriods(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(periodsReverse[S_FREQ_NUMBER_PERIODS]));

    SCPI_PROCESS_ARRAY(periodsReverse, SetNumberPeriods(i)); //-V2563
}


static pString labels[] =
{
    " 100KHZ",
    " 1MHZ",
    " 10MHZ",
    " 100MHZ",
    ""
};

static void SetLabels(int i)
{
    if (S_FREQ_MODE_MEASURE_IS_PERIOD)
    {
        S_FREQ_FREQ_CLC = static_cast<FreqMeter::FreqClc::E>(i);
        
        FreqMeter::LoadPeriodSettings();
    }
}

static pCHAR FuncLabels(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(labels[S_FREQ_FREQ_CLC]));

    SCPI_PROCESS_ARRAY(labels, SetLabels(i)); //-V2563
}


static pString measures[] =
{
    " FREQUENCY",
    " PERIOD",
    ""
};

static void SetMeasure(int i)
{
    S_FREQ_MODE_MEASURE = static_cast<FreqMeter::ModeMeasure::E>(i);
    PageFreqMeter::Init();
}

static pCHAR FuncMeasure(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(measures[S_FREQ_MODE_MEASURE]));

    SCPI_PROCESS_ARRAY(measures, SetMeasure(i)); //-V2563
}


static pString mode[] =
{
    " OFF",
    " ON",
    ""
};

static void SetMode(int i)
{
    S_FREQ_METER_ENABLED = (i == 1);
    FreqMeter::Init();
}


static pCHAR FuncMode(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(S_FREQ_METER_ENABLED ? " ON" : " OFF"));

    SCPI_PROCESS_ARRAY(mode, SetMode(i)); //-V2563
}


static pString time[] =
{
    " 100MS",
    " 1S",
    " 10S",
    ""
};

static void SetTime(int i)
{
    if(S_FREQ_MODE_MEASURE_IS_FREQUENCY)
    {
        S_FREQ_TIME_COUNTING = static_cast<FreqMeter::TimeCounting::E>(i);
    
        FreqMeter::LoadFreqSettings();
    }
}

static pCHAR FuncTime(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(time[S_FREQ_TIME_COUNTING]));

    SCPI_PROCESS_ARRAY(time, SetTime(i)); //-V2563
}


static pCHAR FuncValue(pCHAR buffer) //-V2506
{
    SCPI_PROLOG(buffer);

    SCPI::Sender::freqMeter = true;

    SCPI_EPILOG(buffer);
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
