#include "defines.h"
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


bool FreqMeterC::GetFlag::FREQ_READY()
{
    return FPGA::GetFlag::FREQ_READY();
}

bool FreqMeterC::GetFlag::PERIOD_READY()
{
    return FPGA::GetFlag::PERIOD_READY();
}

bool FreqMeterC::GetFlag::FREQ_OVERFLOW()
{
    return FPGA::GetFlag::FREQ_OVERFLOW();
}

bool FreqMeterC::GetFlag::PERIOD_OVERFLOW()
{
    return FPGA::GetFlag::PERIOD_OVERFLOW();
}

bool FreqMeterC::GetFlag::FREQ_IN_PROCESS()
{
    return FPGA::GetFlag::FREQ_IN_PROCESS();
}

bool FreqMeterC::GetFlag::PERIOD_IN_PROCESS()
{
    return FPGA::GetFlag::PERIOD_IN_PROCESS();
}

void RecorderC::LoadRegUPR()
{
    FPGA::LoadRegUPR();
}

void TesterC::OnPressStart()
{
    FPGA::OnPressStart();
}

bool TesterC::Start()
{
    return FPGA::ForTester::Start();
}

bool TesterC::Read(uint16 *dataA, uint8 *dataB)
{
    return FPGA::ForTester::Read(dataA, dataB);
}
