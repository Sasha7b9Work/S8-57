#include "defines.h"
#include "device.h"
#include "log.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_HAL.h"
#include "FPGA/FPGA_Settings.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include <cstring>
#include <stdlib.h>

#include "Recorder/Recorder.h"
#include "Osci/Osci_Storage.h"
#include "Data/Reader.h"


using namespace HAL::ADDRESSES::FPGA;
using namespace FPGA::HAL;
using namespace FPGA::Settings;
using namespace Osci::Settings;

using HAL::FSMC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16 FPGA::valueADC = 0;

uint16 FPGA::post = (uint16)~(512);
uint16 FPGA::pred = (uint16)~(512);

uint8 dataRand[Chan::Size][FPGA::MAX_NUM_POINTS];    ///< Здесь будут данные рандомизатора

bool          FPGA::isRunning = false;
uint          FPGA::timeStart = 0;
StateWorkFPGA FPGA::fpgaStateWork = StateWorkFPGA_Stop;

/// True, если дан запуск
bool givingStart = false;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class DataAccessor
    {
    public:
        static uint8 *DataA(Osci::Data *data)
        {
            return data->dataA;
        }
        static uint8 *DataB(Osci::Data *data)
        {
            return data->dataB;
        }
    };
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::GiveStart()
{
    uint8 value = (uint8)(((uint8)TRIG_POLARITY) % 2);

    uint8 stop = 0;
    if (Device::State::InModeRecorder())    // В режиме регистратора
    {
        stop = (1 << BIT_TRIG_ENABLED);     // устанавливаем признак того, что процесс чтения данных бесконечен
    }

    FSMC::WriteToFPGA8(WR::TRIG, (uint8)(value++ | stop));
    FSMC::WriteToFPGA8(WR::TRIG, (uint8)((value % 2) | stop));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16 FPGA::ReadLastRecord(Chan::E ch)
{
    static uint16 address = 0;

    if (Chan(ch).IsA())
    {
        address = (uint16)(FSMC::ReadFromFPGA(RD::LAST_RECORD_LO) + ((FSMC::ReadFromFPGA(RD::LAST_RECORD_HI)) << 8));
    }

    return address;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ForTester::Start() // -V2506
{
    // У нас двенадцать делений. На двенадцать делений должно приходиться не менее 2.5 мс
    // 2.5мс / 12дел = 0.2 мс/дел = 10мкс/тчк

    SET_TBASE = TBase::_500us;

    TBase::Load();
    
    FSMC::WriteToFPGA16(WR::POST_LO, (uint16)(~(400 + 1)));
    FSMC::WriteToFPGA16(WR::PRED_LO, (uint16)(~(0+ 3)));
    FSMC::WriteToFPGA8(WR::START, 0xff);

    uint start = TIME_US;
    HAL::flag = 0;
    while (!HAL::GetFlag::PRED())
    {
        HAL::ReadFlag();
        if(TIME_US - start > 1000) /// \todo Временная затычка. Надо сделать так, чтобы такие ситуации были исключены. Сбои происходят, во время
        {                               /// нажатия кнопок
            return;
        }
    }

    GiveStart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::ForTester::Read(uint8 *dataA, uint8 *dataB) // -V2506
{
    uint start = TIME_MS;
    HAL::flag = 0;
    while (!HAL::GetFlag::DATA_READY())    // Ждём флага готовности данных
    {
        HAL::ReadFlag();

        if(TIME_MS - start > 20)        /// \todo Временная затычка. Надо сделать так, чтобы такие ситуации были исключены. Сбои происходят, во время
        {                               /// нажатия кнопок
            return false;
        }
    }

    uint16 aRead = (uint16)(ReadLastRecord(Chan::A) - TESTER_NUM_POINTS);

    FSMC::WriteToFPGA16(WR::PRED_LO, aRead);             // Указываем адрес, с которого будем читать данные
    FSMC::WriteToFPGA8(WR::START_ADDR, 0xff);            // И даём команду ПЛИС, чтобы чтение начиналось с него

    uint8 *addrA = RD::DATA_A; // -V566
    addrA++;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // Читаем данные первого канала
    {
        *dataA++ = *addrA;
    }

    FSMC::WriteToFPGA16(WR::PRED_LO, aRead);             // Указываем адрес, с котонрого будем читать данные
    FSMC::WriteToFPGA8(WR::START_ADDR, 0xff);            // И даём команду ПЛИС, чтобы чтение начиналось с него

    uint8 *addrB = RD::DATA_B; // -V566
    addrB++;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // Читаем данные второго канала
    {
        *dataB++ = *addrB;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Reset()
{
    TShift::Load();

    HAL::LoadRegUPR();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint FPGA::BytesInChannel()
{
    Osci::Settings::Memory::ENumPointsFPGA points(FPGA_ENUM_POINTS);
    return points.BytesInChannel(SET_PEAKDET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetValueADC(uint16 value)
{
    valueADC = value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::IsRunning()
{
    return isRunning;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ClearDataRand()
{
    std::memset(dataRand, 0, FPGA::MAX_NUM_POINTS * 2 * sizeof(uint8));  // -V512
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void AverageData(Chan::E ch, const uint8 *dataNew, int size)
{
    uint8 *_new = (uint8 *)dataNew;
    uint16 *av = AVE_DATA(ch);

    uint16 numAve = (uint16)ENUM_AVE;

    for (int i = 0; i < size; i++)
    {
        av[i] = (uint16)(av[i] - (av[i] >> numAve));

        av[i] += *_new;

        *_new = (uint8)(av[i] >> numAve);

        _new++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadData()
{
    Osci::Data *data = Osci::Storage::PrepareForNewData();

    ReadDataChanenl(Chan::A, DataAccessor::DataA(data));
    ReadDataChanenl(Chan::B, DataAccessor::DataB(data));

    if (ENUM_AVE != Display::ENumAverage::_1)               // Если включено усреднение
    {
        Osci::Data *last = Osci::Storage::GetData(0);
        Osci::Data *prev = Osci::Storage::GetData(1);

        if (prev && last)
        {
            const DataSettings *setLast = last->Settings();
            const DataSettings *setPrev = prev->Settings();

            if (setLast->Equals(*setPrev))
            {
                if (ENABLED_A(setLast))
                {
                    AverageData(Chan::A, last->DataA(), setLast->SizeChannel());
                }
                if (ENABLED_B(setPrev))
                {
                    AverageData(Chan::B, last->DataB(), setLast->SizeChannel());
                }
            }
        }
    }
}
