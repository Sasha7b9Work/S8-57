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


static pString measures[] =
{
    " AC2V",
    " AC20V",
    " AC400V",
    " DC2V",
    " DC20V",
    " DC400V",
    " AC20MA",
    " AC2A",
    " DC20MA",
    " DC2A",
    " 2K",
    " 20K",
    " 200K",
    " 10M",
    " DIODE",
    " BELL",
    ""
};


static void SendAnswerForMeasure()
{
    uint8 meas = static_cast<uint8>(set.mult._meas);

    if (meas == 0)                     // DC Voltage
    {
        static const pString ranges[] =  { " DC2V", " DC20V", " DC400V" };
        SCPI::SendAnswer(ranges[set.mult._rangeVoltageDC]);
    }
    else if (meas == 1)                 // AC Voltage
    {
        static const pString ranges[] = { " AC2V", " AC20V", " AC400V" };
        SCPI::SendAnswer(ranges[set.mult._rangeVoltageAC]);
    }
    else if (meas == 2)
    {
        SCPI::SendAnswer(set.mult._rangeCurrentDC == 0 ? " DC20MA" : " DC2A");
    }
    else if (meas == 3)
    {
        SCPI::SendAnswer(set.mult._rangeCurrentAC == 0 ? " AC20MA" : " AC2A");
    }
    else if (meas == 4)
    {
        static const pString ranges[] = { "2K", "20K", "200K", "10M" };
        SCPI::SendAnswer(ranges[set.mult._rangeResist]);
    }
    else if (meas == 5)
    {
        SCPI::SendAnswer(" DIODE");
    }
    else if (meas == 6)
    {
        SCPI::SendAnswer(" BELL");
    }
}


static void EnableMeasure(int i)
{
    struct StructMode
    {
        uint8 mode;
        uint8 range;
        uint8 *pRange;
    };

    static const StructMode modes[] =
    {
        {1, 0, reinterpret_cast<uint8 *>(&set.mult._rangeVoltageAC)},   // " AC2V",
        {1, 1, reinterpret_cast<uint8 *>(&set.mult._rangeVoltageAC)},   // " AC20V",
        {1, 2, reinterpret_cast<uint8 *>(&set.mult._rangeVoltageAC)},   // " AC400V",
        {0, 0, reinterpret_cast<uint8 *>(&set.mult._rangeVoltageDC)},   // " DC2V",
        {0, 1, reinterpret_cast<uint8 *>(&set.mult._rangeVoltageDC)},   // " DC20V",
        {0, 2, reinterpret_cast<uint8 *>(&set.mult._rangeVoltageDC)},   // " DC400V",
        {3, 0, reinterpret_cast<uint8 *>(&set.mult._rangeCurrentAC)},   // " AC20MA",
        {3, 1, reinterpret_cast<uint8 *>(&set.mult._rangeCurrentAC)},   // " AC2A",
        {2, 0, reinterpret_cast<uint8 *>(&set.mult._rangeCurrentDC)},   // " DC20MA",
        {2, 1, reinterpret_cast<uint8 *>(&set.mult._rangeCurrentDC)},   // " DC2A",
        {4, 0, reinterpret_cast<uint8 *>(&set.mult._rangeResist)},      // " 2K",
        {4, 1, reinterpret_cast<uint8 *>(&set.mult._rangeResist)},      // " 20K",
        {4, 2, reinterpret_cast<uint8 *>(&set.mult._rangeResist)},      // " 200K",
        {4, 3, reinterpret_cast<uint8 *>(&set.mult._rangeResist)},      // " 10M",
        {5, 0, nullptr},                                                // " DIODE",
        {6, 0, nullptr}                                                 // " BELL"
    };

    const StructMode &mode = modes[i];

    if(mode.pRange != nullptr)
    {
        *mode.pRange = mode.range;
    }
    set.mult._meas = static_cast<MultimeterMeasure::E>(mode.mode);

    PageMultimeter::OnChanged_Mode();
}


static pCHAR FuncMeasure(pCHAR buffer)
{
    SCPI_REQUEST(SendAnswerForMeasure());

    SCPI_PROCESS_ARRAY(measures, EnableMeasure(i));
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
