#include "defines.h"
#include "FPGA/FPGA.h"


uint16 &OsciC::pred = _FPGA::pred;
uint16 &OsciC::post = _FPGA::post;
uint &OsciC::timeStart = _FPGA::timeStart;
bool &OsciC::isRunning = _FPGA::isRunning;


void OsciC::LoadRegUPR()
{
    _FPGA::LoadRegUPR();
}

void OsciC::LoadCalibratorMode()
{
    _FPGA::LoadCalibratorMode();
}

void OsciC::OnPressStart()
{
    _FPGA::OnPressStart();
}

bool OsciC::IsRunning()
{
    return _FPGA::IsRunning();
}

void OsciC::ReadFlag()
{
    _FPGA::ReadFlag();
}

void OsciC::GiveStart()
{
    _FPGA::GiveStart();
}

void OsciC::ReadData()
{
    _FPGA::ReadData();
}

void OsciC::Reset()
{
    _FPGA::Reset();
}

void OsciC::SetValueADC(uint16 value)
{
    _FPGA::SetValueADC(value);
}

uint OsciC::BytesInChannel()
{
    return _FPGA::BytesInChannel();
}

bool OsciC::GetFlag::PRED()
{
    return _FPGA::GetFlag::PRED();
}

bool OsciC::GetFlag::HOLD_OFF()
{
    return _FPGA::GetFlag::HOLD_OFF();
}

bool OsciC::GetFlag::TRIG_READY()
{
    return _FPGA::GetFlag::TRIG_READY();
}

bool OsciC::GetFlag::DATA_READY()
{
    return _FPGA::GetFlag::DATA_READY();
}


bool FreqMeterC::GetFlag::FREQ_READY()
{
    return _FPGA::GetFlag::FREQ_READY();
}

bool FreqMeterC::GetFlag::PERIOD_READY()
{
    return _FPGA::GetFlag::PERIOD_READY();
}

bool FreqMeterC::GetFlag::FREQ_OVERFLOW()
{
    return _FPGA::GetFlag::FREQ_OVERFLOW();
}

bool FreqMeterC::GetFlag::PERIOD_OVERFLOW()
{
    return _FPGA::GetFlag::PERIOD_OVERFLOW();
}

bool FreqMeterC::GetFlag::FREQ_IN_PROCESS()
{
    return _FPGA::GetFlag::FREQ_IN_PROCESS();
}

bool FreqMeterC::GetFlag::PERIOD_IN_PROCESS()
{
    return _FPGA::GetFlag::PERIOD_IN_PROCESS();
}

void RecorderC::LoadRegUPR()
{
    _FPGA::LoadRegUPR();
}

void TesterC::OnPressStart()
{
    _FPGA::OnPressStart();
}

bool TesterC::Start()
{
    return _FPGA::ForTester::Start();
}

bool TesterC::Read(uint16 *dataA, uint8 *dataB)
{
    return _FPGA::ForTester::Read(dataA, dataB);
}
