#include "defines.h"
#include "Device.h"
#include "SCPI/SCPI.h"


// :MULTIMETER:AVP
static pCHAR FuncAVP(pCHAR);
static void HintAVP(String *);
static bool TestAVP();

// :MULTIMETER:MEASURE
static pCHAR FuncMeasure(pCHAR);
static void HintMeasure(String *);
static bool TestMeasure();

// :MULTIMETER:MODE
static pCHAR FuncMode(pCHAR);
static void HintMode(String *);
static bool TestMode();

// :MULTIMETER:VALUE?
static pCHAR FuncValue(pCHAR);
static void HintValue(String *);
static bool TestValue();

// :MULTIMETER:ZERO
static pCHAR FuncZero(pCHAR);
static void HintZero(String *);
static bool TestZero();


const StructSCPI SCPI::multimeter[] =
{
    SCPI_LEAF(":AVP",     FuncAVP,     TestAVP,     "", HintAVP),
    SCPI_LEAF(":MEASURE", FuncMeasure, TestMeasure, "", HintMeasure),
    SCPI_LEAF(":MODE",    FuncMode,    TestMode,    "", HintMode),
    SCPI_LEAF(":VALUE?",  FuncValue,   TestValue,   "", HintValue),
    SCPI_LEAF(":ZERO",    FuncZero,    TestZero,    "", HintZero),
    SCPI_EMPTY()
};


static pCHAR FuncAVP(pCHAR)
{
    return nullptr;
}


static pCHAR FuncMeasure(pCHAR)
{
    return nullptr;
}


static void EnableMultimeter()
{
    if (Menu::OpenedPage() != PageMultimeter::self)
    {
        Keyboard::ShortPress(Key::Function);
        Keyboard::ShortPress(Key::Enter);
        Keyboard::ShortPress(Key::Enter);
        Keyboard::ShortPress(Key::Enter);
        Keyboard::ShortPress(Key::Function);
        Keyboard::ShortPress(Key::F3);
    }
}


static void DisableMultimeter()
{
    if (Menu::OpenedPage() == PageMultimeter::self)
    {
        Keyboard::ShortPress(Key::Enter);
        Keyboard::ShortPress(Key::Enter);
    }
}


static pCHAR FuncMode(pCHAR buffer)
{
    String answer("%s", Device::InModeMultimeter() ? " ON" : " OFF");
    SCPI_REQUEST(SCPI::SendAnswer(answer.c_str()));

    SCPI_IF_BEGIN_WITH_THEN(" ON", EnableMultimeter);

    SCPI_IF_BEGIN_WITH_THEN(" OFF", DisableMultimeter);

    return nullptr;
}


static pCHAR FuncValue(pCHAR)
{
    return nullptr;
}


static pCHAR FuncZero(pCHAR)
{
    return nullptr;
}


static bool TestAVP()
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


static bool TestValue()
{
    return false;
}


static bool TestZero()
{
    return false;
}


static void HintAVP(String *)
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


static void HintZero(String *)
{

}
