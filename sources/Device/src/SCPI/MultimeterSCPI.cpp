#include "defines.h"
#include "Device.h"
#include "SCPI/SCPI.h"


namespace S_MULT
{
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

    static void EnableMultimeter();
    static void DisableMultimeter();
    static void TuneAVP(int);


    void S_MULT::TuneAVP(int i)
    {
        if (Device::InModeMultimeter())
        {
            set.mult._avp = static_cast<ModeAVP::E>(i);
            Multimeter::ChangeAVP();
        }
    }


    pCHAR S_MULT::FuncAVP(pCHAR buffer)
    {
        static pString avp[] =
        {
            " OFF",
            " ON",
            ""
        };

        SCPI_REQUEST(SCPI::SendAnswer(String(avp[set.mult._avp]).c_str()));

        SCPI_PROCESS_ARRAY(avp, TuneAVP(i));
    }


    static pString measures[] =
    {
        " AC2V",
        " AC20V",
        " AC400V",
        " DC2V",
        " DC20V",
        " DC500V",
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
            static const pString ranges[] = { " DC2V", " DC20V", " DC400V" };
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
        else if (meas == 6) //-V2516
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

        if (mode.pRange != nullptr)
        {
            *mode.pRange = mode.range;
        }
        set.mult._meas = static_cast<MultimeterMeasure::E>(mode.mode);

        PageMultimeter::OnChanged_Mode();
    }


    pCHAR S_MULT::FuncMeasure(pCHAR buffer)
    {
        SCPI_REQUEST(SendAnswerForMeasure());

        SCPI_PROCESS_ARRAY(measures, EnableMeasure(i));
    }


    void S_MULT::EnableMultimeter()
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


    void S_MULT::DisableMultimeter()
    {
        if (Menu::OpenedPage() == PageMultimeter::self)
        {
            Keyboard::ShortPress(Key::Enter);
            Keyboard::ShortPress(Key::Enter);
        }
    }


    pCHAR S_MULT::FuncMode(pCHAR buffer)
    {
        SCPI_REQUEST(SCPI::SendAnswer(Device::InModeMultimeter() ? " ON" : " OFF"));

        SCPI_IF_BEGIN_WITH_THEN(" ON", EnableMultimeter());

        SCPI_IF_BEGIN_WITH_THEN(" OFF", DisableMultimeter());

        return nullptr;
    }


    pCHAR S_MULT::FuncValue(pCHAR buffer)
    {
        SCPI_PROLOG(buffer);

        SCPI::Sender::multimeter = true;

        SCPI_EPILOG(buffer);
    }


    pCHAR S_MULT::FuncZero(pCHAR buffer)
    {
        static pString zero[] =
        {
            " OFF",
            " ON",
            ""
        };

        SCPI_REQUEST(SCPI::SendAnswer(String(PageMultimeter::ZeroEnabled() ? " ON" : " OFF").c_str()));

        SCPI_PROCESS_ARRAY(zero, PageMultimeter::EnableZero(i != 0));
    }


    bool S_MULT::TestAVP()
    {
        return false;
    }


    bool S_MULT::TestMeasure()
    {
        return false;
    }


    bool S_MULT::TestMode()
    {
        return false;
    }


    bool S_MULT::TestValue()
    {
        return false;
    }


    bool S_MULT::TestZero()
    {
        return false;
    }


    void S_MULT::HintAVP(String *)
    {

    }


    void S_MULT::HintMeasure(String *)
    {

    }


    void S_MULT::HintMode(String *)
    {

    }


    void S_MULT::HintValue(String *)
    {

    }


    void S_MULT::HintZero(String *)
    {

    }
}


const StructSCPI SCPI::multimeter[] =
{
    SCPI_LEAF(":AVP",     S_MULT::FuncAVP,     S_MULT::TestAVP,     "", S_MULT::HintAVP),
    SCPI_LEAF(":MEASURE", S_MULT::FuncMeasure, S_MULT::TestMeasure, "", S_MULT::HintMeasure),
    SCPI_LEAF(":MODE",    S_MULT::FuncMode,    S_MULT::TestMode,    "", S_MULT::HintMode),
    SCPI_LEAF(":VALUE?",  S_MULT::FuncValue,   S_MULT::TestValue,   "", S_MULT::HintValue),
    SCPI_LEAF(":ZERO",    S_MULT::FuncZero,    S_MULT::TestZero,    "", S_MULT::HintZero),
    SCPI_EMPTY()
};
