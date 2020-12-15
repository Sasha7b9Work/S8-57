#include "defines.h"
#include "Device.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageTester.h"
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


static pString polarity[] =
{
    " NPN",
    " PNP",
    ""
};


static void SetPolarity(int i)
{
    set.test._polarity = static_cast<Tester::Polarity::E>(i);
    Tester::LoadPolarity();
}


static pCHAR FuncConduction(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(polarity[set.test._polarity]));

    SCPI_PROCESS_ARRAY(polarity, SetPolarity(i));
}


static void SendAnswerForControl()
{
    if (set.test._control == Tester::Control::Current)
    {
        SCPI::SendAnswer(set.test._stepI == Tester::StepI::_4uA ? " 4UA" : " 20UA");
    }
    else
    {
        SCPI::SendAnswer(set.test._stepU == Tester::StepU::_600mV ? " 600MV" : " 3V");
    }
}


static pString controls[] =
{
    " 600MV",
    " 3V",
    " 4UA",
    " 20UA",
    ""
};


static void EnableControl(int i)
{
    if (i < 2)
    {
        set.test._control = Tester::Control::Voltage;
        set.test._stepU = (i == 0) ? Tester::StepU::_600mV : Tester::StepU::_3V;
    }
    else
    {
        set.test._control = Tester::Control::Current;
        set.test._stepI = (i == 2) ? Tester::StepI::_4uA : Tester::StepI::_20uA;
    }

    PageTester::OnChanged_Control();
}


static pCHAR FuncControl(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SendAnswerForControl());

    SCPI_PROCESS_ARRAY(controls, EnableControl(i));
}


static pCHAR FuncData(pCHAR buffer) //-V2506
{
    SCPI_PROLOG(buffer);

    SCPI::Sender::tester = true;

    SCPI_EPILOG(buffer);
}


static void EnableTester()
{
    Keyboard::ShortPress(Key::Function);
    Keyboard::ShortPress(Key::Enter);
    Keyboard::ShortPress(Key::Enter);
    Keyboard::ShortPress(Key::Enter);
    Keyboard::ShortPress(Key::Function);
    Keyboard::ShortPress(Key::F4);
}

static void DisableTester()
{
    if (Menu::OpenedItem() == PageTester::self)
    {
        Keyboard::ShortPress(Key::Enter);
        Keyboard::ShortPress(Key::Enter);
    }
}

static pCHAR FuncMode(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(Device::InModeTester() ? " ON" : " OFF"));

    SCPI_IF_BEGIN_WITH_THEN(" ON", EnableTester());

    SCPI_IF_BEGIN_WITH_THEN(" OFF", DisableTester());

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
