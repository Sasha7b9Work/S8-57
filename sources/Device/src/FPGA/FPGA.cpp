#include "defines.h"
#include "log.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/RAM.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include <cstring>


bool   FPGA::forcedStart = false;
uint16 FPGA::valueADC = 0;
uint16 FPGA::post = static_cast<uint16>(~(512));
uint16 FPGA::pred = static_cast<uint16>(~(512));

bool          FPGA::isRunning = false;


void FPGA::ForcedStart()
{
    uint8 value = static_cast<uint8>((static_cast<uint8>(TrigPolarity())) % 2);

    uint8 stop = 0;
    if (Device::InModeRecorder())           // В режиме регистратора
    {
        stop = (1 << BIT_TRIG_ENABLED);     // устанавливаем признак того, что процесс чтения данных бесконечен
    }

    HAL_BUS::FPGA::Write8(WR::TRIG, static_cast<uint8>(value++ | stop));
    HAL_BUS::FPGA::Write8(WR::TRIG, static_cast<uint8>((value % 2) | stop));

    forcedStart = true;
}


uint16 FPGA::ReadLastRecord(Chan::E ch)
{
    static uint16 address = 0;

    if (Chan(ch).IsA())
    {
        address = static_cast<uint16>(HAL_BUS::FPGA::Read(RD::LAST_RECORD_LO) + ((HAL_BUS::FPGA::Read(RD::LAST_RECORD_HI)) << 8));
    }

    return address;
}


void FPGA::OnPressStart()
{
    if (Device::InModeRecorder())
    {
        Recorder::OnPressStart();
    }
    else
    {
        if (IsRunning())
        {
            Osci::Stop();
        }
        else
        {
            Osci::Start(true);
        }
    }
}


void FPGA::Reset()
{
    TShift().Load();

    FPGA::LoadRegUPR();
}


void FPGA::SetValueADC(uint16 value)
{
    valueADC = value;
}


bool FPGA::IsRunning()
{
    return isRunning;
}


void FPGA::ReadData()
{
    Osci::Stop();

    DataSettings *ds = RAM::PrepareForNewData();

    if (ReadDataChannel(Chan::A, ds->dataA))
    {
        if (ReadDataChannel(Chan::B, ds->dataB))
        {
            for(uint i = 0; i < ds->BytesInChannel(); i++)
            {
                if(ds->dataB[i] < VALUE::AVE - 20)
                {
                    LOG_WRITE("%d - 0x%x", i, ds->dataB[i]);
                }
            }
        }
        else
        {
            return;
        }
    }

    if (ENumAverage() != ENumAverage::_1)               // Если включено усреднение
    {
        DataSettings *last = RAM::Get(0);
        DataSettings *prev = RAM::Get(1);

        if (prev && last)
        {
            if (last->Equals(*prev))
            {
                if (ENABLED_A(last))
                {
                    AveragerOsci::Process(Chan::A, last->dataA, last->BytesInChannel());
                }
                if (ENABLED_B(last))
                {
                    AveragerOsci::Process(Chan::B, last->dataB, last->BytesInChannel());
                }
            }
        }
    }
}


void FPGA::GiveStart(uint16 pr, uint16 po)
{
    HAL_BUS::FPGA::Write16(WR::PRED_LO, pr);
    HAL_BUS::FPGA::Write16(WR::POST_LO, po);
    HAL_BUS::FPGA::Write8(WR::START, 0xff);
}
