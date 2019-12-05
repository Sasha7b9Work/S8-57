#include "defines.h"
#include "FPGA/ContextOsci.h"
#include "FPGA/FPGA.h"


uint16 &ContextOsci::pred = FPGA::pred;
uint16 &ContextOsci::post = FPGA::post;
uint &ContextOsci::timeStart = FPGA::timeStart;
bool &ContextOsci::isRunning = FPGA::isRunning;


void ContextOsci::OnPressStart()
{
    if (ModeWork::IsDir())
    {
        FPGA::OnPressStart();
    }
}

void ContextOsci::Reset()
{
    FPGA::Reset();
}

bool ContextOsci::InStateStop()
{
    return FPGA_IN_STATE_STOP;
}

bool ContextOsci::InStateWait()
{
    return FPGA_IN_STATE_WAIT;
}

void ContextOsci::SetValueADC(uint16 value)
{
    FPGA::SetValueADC(value);
}

uint ContextOsci::BytesInChannel()
{
    return ENumPointsFPGA().BytesInChannel();
}

bool ContextOsci::GetFlag::PRED()
{
    return FPGA::flag.Pred();
}

bool ContextOsci::GetFlag::HOLD_OFF()
{
    return FPGA::flag.HoldOff();
}

bool ContextOsci::GetFlag::TRIG_READY()
{
    return FPGA::flag.TrigReady();
}

bool ContextOsci::GetFlag::DATA_READY()
{
    return FPGA::flag.DataReady();
}
