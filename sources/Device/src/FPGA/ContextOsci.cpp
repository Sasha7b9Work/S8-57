#include "defines.h"
#include "FPGA/ContextOsci.h"
#include "FPGA/FPGA.h"


uint16 &OsciC::pred = FPGA::pred;
uint16 &OsciC::post = FPGA::post;
uint &OsciC::timeStart = FPGA::timeStart;
bool &OsciC::isRunning = FPGA::isRunning;


void OsciC::LoadRegUPR()
{
    FPGA::LoadRegUPR();
}

void OsciC::LoadCalibratorMode()
{
    FPGA::LoadCalibratorMode();
}

void OsciC::OnPressStart()
{
    FPGA::OnPressStart();
}

bool OsciC::IsRunning()
{
    return FPGA::IsRunning();
}

void OsciC::ReadFlag()
{
    FPGA::ReadFlag();
}

void OsciC::GiveStart()
{
    FPGA::GiveStart();
}

void OsciC::ReadData()
{
    FPGA::ReadData();
}

void OsciC::Reset()
{
    FPGA::Reset();
}

bool OsciC::InStateStop()
{
    return FPGA_IN_STATE_STOP;
}

bool OsciC::InStateWait()
{
    return FPGA_IN_STATE_WAIT;
}

void OsciC::SetValueADC(uint16 value)
{
    FPGA::SetValueADC(value);
}

uint OsciC::BytesInChannel()
{
    return FPGA::BytesInChannel();
}

bool OsciC::GetFlag::PRED()
{
    return FPGA::GetFlag::PRED();
}

bool OsciC::GetFlag::HOLD_OFF()
{
    return FPGA::GetFlag::HOLD_OFF();
}

bool OsciC::GetFlag::TRIG_READY()
{
    return FPGA::GetFlag::TRIG_READY();
}

bool OsciC::GetFlag::DATA_READY()
{
    return FPGA::GetFlag::DATA_READY();
}
