#include "defines.h"
#include "SCPI/SCPI.h"


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


static pCHAR FuncDisplay(pCHAR)
{
    return nullptr;
}


static pCHAR FuncSource(pCHAR)
{
    return nullptr;
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
