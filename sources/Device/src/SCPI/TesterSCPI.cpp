#include "defines.h"
#include "SCPI/SCPI.h"

// :TESTER:CONDUCTION
static pCHAR FuncConduction(pCHAR);
static void HintConduction(String *);
static bool TestConduction();

// :TESTER:CONTROL
static pCHAR FuncControl(pCHAR);
static void HintControl(String *);
static bool TestControl();

// :TESTER:DATA?
static pCHAR FuncData(pCHAR);
static void HintData(String *);
static bool TestData();

// :TESTER:MODE
static pCHAR FuncMode(pCHAR);
static void HintMode(String *);
static bool TestMode();


const StructSCPI SCPI::tester[] =
{
    SCPI_LEAF(":CONDUCTION", FuncConduction, TestConduction, "", HintConduction),
    SCPI_LEAF(":CONTROL",    FuncControl,    TestControl,    "", HintControl),
    SCPI_LEAF(":DATA?",      FuncData,       TestData,       "", HintData),
    SCPI_LEAF(":MODE",       FuncMode,       TestMode,       "", HintMode),
    SCPI_EMPTY()
};


static pCHAR FuncConduction(pCHAR)
{
    return nullptr;
}


static pCHAR FuncControl(pCHAR)
{
    return nullptr;
}


static pCHAR FuncData(pCHAR)
{
    return nullptr;
}


static pString mode[] =
{
    " ON",
    " OFF",
    ""
};

static pCHAR FuncMode(pCHAR buffer)
{
    //SCPI_REQUEST(SCPI::SendAnswer(mode[set.test._viewMode))
    return nullptr;
}


static void HintConduction(String *)
{

}


static void HintControl(String *)
{

}


static void HintData(String *)
{

}


static void HintMode(String *)
{

}


static bool TestConduction()
{
    return false;
}


static bool TestControl()
{
    return false;
}


static bool TestData()
{
    return false;
}


static bool TestMode()
{
    return false;
}
