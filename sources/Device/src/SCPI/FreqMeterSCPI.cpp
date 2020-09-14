#include "defines.h"
#include "SCPI/SCPI.h"


// :MEASURE:MODE
static pCHAR FuncMode(pCHAR);
static void HintMode(String *);
static bool TestMode();


const StructSCPI SCPI::freqmeter[] =
{
    SCPI_LEAF(":MODE", FuncMode, TestMode, "", HintMode),
    SCPI_EMPTY()
};


static pCHAR FuncMode(pCHAR)
{
    return nullptr;
}


static void HintMode(String *)
{
}


static bool TestMode()
{
    return false;
}
