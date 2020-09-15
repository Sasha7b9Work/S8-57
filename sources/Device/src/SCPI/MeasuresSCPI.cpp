#include "defines.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


// :MEASURE:DISPLAY
static pCHAR FuncDisplay(pCHAR);
static void HintDisplay(String *);
static bool TestDisplay();

// :MEASURE:SOURCE
static pCHAR FuncSource(pCHAR);
static void HintSource(String *);
static bool TestSource();

// :MEASURE:VALUE
static pCHAR FuncValue(pCHAR);
static void HintValue(String *);
static bool TestValue();


const StructSCPI SCPI::measures[] =
{
    SCPI_LEAF(":DISPLAY", FuncDisplay, TestDisplay, "", HintDisplay),
    SCPI_LEAF(":SOURCE",  FuncSource,  TestSource,  "", HintSource),
    SCPI_LEAF(":VALUE",   FuncValue,   TestValue,   "", HintValue),
    SCPI_EMPTY()
};


static pString display[] =
{
    " OFF",
    " ON",
    ""
};

static void DisplayMeasures(int i)
{
    S_MEAS_SHOW = (i == 1);
}

static pCHAR FuncDisplay(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(S_MEAS_SHOW ? " ON" : " OFF"));

    SCPI_PROCESS_ARRAY(display, DisplayMeasures(i));
}


static pString source[] =
{
    " 1",
    " 2",
    " BOTH",
    ""
};

static pCHAR FuncSource(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(source[S_MEAS_SOURCE]));

    SCPI_PROCESS_ARRAY(source, S_MEAS_SOURCE = static_cast<MeasuresSource::E>(i));
}


static pCHAR FuncValue(pCHAR)
{
    return nullptr;
}


static void HintDisplay(String *)
{

}


static void HintSource(String *)
{

}


static void HintValue(String *)
{

}


static bool TestDisplay()
{
    return false;
}


static bool TestSource()
{
    return false;
}


static bool TestValue()
{
    return false;
}
