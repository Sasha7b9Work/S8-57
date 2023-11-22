// 2023/11/21 10:10:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageTester.h"
#include "SCPI/SCPI.h"


namespace S_TESTER
{
    // :TESTER:CONDUCTION
    static pCHAR FuncConduction(pCHAR);
    static void HintConduction(String *);
    static bool TestConduction();

    // :TESTER:CONTROL
    static pCHAR FuncControl(pCHAR);
    static void HintControl(String *);
    static bool TestControl();
    static void SendAnswerForControl();
    static void EnableControl(int);

    // :TESTER:DATA?
    static pCHAR FuncData(pCHAR);
    static void HintData(String *);
    static bool TestData();

    // :TESTER:MODE
    static pCHAR FuncMode(pCHAR);
    static void HintMode(String *);
    static bool TestMode();

    static void SetPolarity(int);
    static void EnableTester();
    static void DisableTester();
}


const StructSCPI SCPI::tester[] =
{
    SCPI_LEAF(":CONDUCTION", S_TESTER::FuncConduction, S_TESTER::TestConduction, "", S_TESTER::HintConduction),
    SCPI_LEAF(":CONTROL",    S_TESTER::FuncControl,    S_TESTER::TestControl,    "", S_TESTER::HintControl),
    SCPI_LEAF(":DATA?",      S_TESTER::FuncData,       S_TESTER::TestData,       "", S_TESTER::HintData),
    SCPI_LEAF(":MODE",       S_TESTER::FuncMode,       S_TESTER::TestMode,       "", S_TESTER::HintMode),
    SCPI_EMPTY()
};


static pString polarity[] =
{
    " NPN",
    " PNP",
    ""
};


void S_TESTER::SetPolarity(int i)
{
    set.test._polarity = static_cast<Tester::Polarity::E>(i);
    Tester::LoadPolarity();
}


pCHAR S_TESTER::FuncConduction(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(polarity[set.test._polarity]));

    SCPI_PROCESS_ARRAY(polarity, SetPolarity(i));
}


void S_TESTER::SendAnswerForControl()
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


void S_TESTER::EnableControl(int i)
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


pCHAR S_TESTER::FuncControl(pCHAR buffer)
{
    SCPI_REQUEST(SendAnswerForControl());

    SCPI_PROCESS_ARRAY(controls, EnableControl(i));
}


pCHAR S_TESTER::FuncData(pCHAR buffer)
{
    SCPI_PROLOG(buffer);

    SCPI::Sender::tester = true;

    SCPI_EPILOG(buffer);
}


void S_TESTER::EnableTester()
{
    Keyboard::ShortPress(Key::Function);
    Keyboard::ShortPress(Key::Enter);
    Keyboard::ShortPress(Key::Enter);
    Keyboard::ShortPress(Key::Enter);
    Keyboard::ShortPress(Key::Function);
    Keyboard::ShortPress(Key::F4);
}

void S_TESTER::DisableTester()
{
    if (Menu::OpenedItem() == PageTester::self)
    {
        Keyboard::ShortPress(Key::Enter);
        Keyboard::ShortPress(Key::Enter);
    }
}

pCHAR S_TESTER::FuncMode(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(Device::InModeTester() ? " ON" : " OFF"));

    SCPI_IF_BEGIN_WITH_THEN(" ON", EnableTester());

    SCPI_IF_BEGIN_WITH_THEN(" OFF", DisableTester());

    return nullptr;
}


void S_TESTER::HintConduction(String *)
{

}


void S_TESTER::HintControl(String *)
{

}


void S_TESTER::HintData(String *)
{

}


void S_TESTER::HintMode(String *)
{

}


bool S_TESTER::TestConduction()
{
    return false;
}


bool S_TESTER::TestControl()
{
    return false;
}


bool S_TESTER::TestData()
{
    return false;
}


bool S_TESTER::TestMode()
{
    return false;
}
