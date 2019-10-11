#include "defines.h"
#include "device.h"
#include "log.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Osci/StorageOsci.h"
#include "Data/Reader.h"
#include <cstring>
#include <stdlib.h>


uint16 FPGA::valueADC = 0;
uint16 FPGA::post = static_cast<uint16>(~(512));
uint16 FPGA::pred = static_cast<uint16>(~(512));

uint8 dataRand[Chan::Count][FPGA::MAX_NUM_POINTS];    ///< Здесь будут данные рандомизатора

bool          FPGA::isRunning = false;
uint          FPGA::timeStart = 0;
StateWorkFPGA FPGA::fpgaStateWork = StateWorkFPGA_Stop;

/// True, если дан запуск
bool givingStart = false;


void FPGA::GiveStart()
{
    uint8 value = static_cast<uint8>((static_cast<uint8>(set.trig.polarity)) % 2);

    uint8 stop = 0;
    if (Device::State::InModeRecorder())    // В режиме регистратора
    {
        stop = (1 << BIT_TRIG_ENABLED);     // устанавливаем признак того, что процесс чтения данных бесконечен
    }

    HAL_FSMC::WriteToFPGA8(WR::TRIG, static_cast<uint8>(value++ | stop));
    HAL_FSMC::WriteToFPGA8(WR::TRIG, static_cast<uint8>((value % 2) | stop));
}


uint16 FPGA::ReadLastRecord(Chan::E ch)
{
    static uint16 address = 0;

    if (Chan(ch).IsA())
    {
        address = static_cast<uint16>(HAL_FSMC::ReadFromFPGA(RD::LAST_RECORD_LO) + ((HAL_FSMC::ReadFromFPGA(RD::LAST_RECORD_HI)) << 8));
    }

    return address;
}


void FPGA::OnPressStart()
{
    if (Device::State::InModeRecorder())
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
            Osci::Start();
        }
    }
}


void FPGA::Reset()
{
    TShift::Load();

    FPGA::LoadRegUPR();
}


uint FPGA::BytesInChannel()
{
    ENumPointsFPGA points(set.mem.enumPoints);
    return points.BytesInChannel(set.time.peakDet);
}


void FPGA::SetValueADC(uint16 value)
{
    valueADC = value;
}


bool FPGA::IsRunning()
{
    return isRunning;
}


void FPGA::ClearDataRand()
{
    std::memset(dataRand, 0, FPGA::MAX_NUM_POINTS * 2 * sizeof(uint8));  // -V512
}


void FPGA::ReadData()
{
    DataOsci *data = StorageOsci::PrepareForNewData();

    if (ReadDataChanenl(Chan::A, data->dataA))
    {
        if (ReadDataChanenl(Chan::B, data->dataB))
        {
        }
        else
        {
            return;
        }
    }

    if (set.disp.ENumAverage != ENumAverage::_1)               // Если включено усреднение
    {
        DataOsci *last = StorageOsci::GetData(0);
        DataOsci *prev = StorageOsci::GetData(1);

        if (prev && last)
        {
            const DataSettings *setLast = last->Settings();
            const DataSettings *setPrev = prev->Settings();

            if (setLast->Equals(*setPrev))
            {
                if (ENABLED_A(setLast))
                {
                    AveragerOsci::Process(Chan::A, last->DataA(), setLast->SizeChannel());
                }
                if (ENABLED_B(setLast))
                {
                    AveragerOsci::Process(Chan::B, last->DataB(), setLast->SizeChannel());
                }
            }
        }
    }
}
