#include "stdafx.h"
#ifndef WIN32
#include <stm32f4xx.h>
#include "defines.h"
#include "device.h"
#include "log.h"
#include "Data/DataBuffer.h"
#include "Data/DataStorage.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_HAL.h"
#include "FPGA/FPGA_Settings.h"
#include "Hardware/FSMC.h"
#include "Hardware/GPIO.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include <cstring>
#include <stdlib.h>

#include "Recorder/Recorder.h"
#endif


using namespace FPGA::ADDR;
using namespace FPGA::HAL::GPIO;
using namespace FPGA::Settings;
using namespace Osci::Settings;


extern void ReadDataChanenlRand(Chan::E ch, const uint8 *address, uint8 *data);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16 FPGA::valueADC = 0;

uint16 FPGA::post = (uint16)~(512);
uint16 FPGA::pred = (uint16)~(512);

uint8 dataRand[Chan::Number][FPGA::MAX_NUM_POINTS];    ///< Здесь будут данные рандомизатора
/// Здесь хранится адрес, начиная с которого будем читать данные по каналам. Если addrRead == 0xffff, то адрес вначале нужно считать
static uint16 addrRead = 0xffff;

extern const int *Kr; //-V707

bool          FPGA::isRunning = false;
uint          FPGA::timeStart = 0;
StateWorkFPGA FPGA::fpgaStateWork = StateWorkFPGA_Stop;
FPGA::State   FPGA::state = {StateWorkFPGA_Stop, StateCalibration_None};

/// True, если дан запуск
bool givingStart = false;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::GiveStart()
{
    uint8 value = (uint8)TRIG_POLARITY;
    value = (uint8)((value + 1) % 2);

    uint8 stop = 0;
    if (Device::State::InModeRecorder())    // В режиме регистратора
    {
        stop = (1 << BIT_TRIG_ENABLED);     // устанавливаем признак того, что процесс чтения данных бесконечен
    }

    FSMC::WriteToFPGA8(WR::TRIG, (uint8)(value++ | stop));
    FSMC::WriteToFPGA8(WR::TRIG, (uint8)((value % 2) | stop));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16 FPGA::ReadLastRecord()
{
    return (uint16)(FSMC::ReadFromFPGA(RD::LAST_RECORD_LO) + ((FSMC::ReadFromFPGA(RD::LAST_RECORD_HI)) << 8));
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
void Osci::Start()
{
    givingStart = false;
    addrRead = 0xffff;

    FSMC::WriteToFPGA16(WR::PRED_LO, FPGA::pred);
    FSMC::WriteToFPGA16(WR::POST_LO, FPGA::post);
    FSMC::WriteToFPGA8(WR::START, 0xff);

    FPGA::timeStart = TIME_MS;

    FPGA::isRunning = true;
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

    uint16 aRead = (uint16)(ReadLastRecord() - TESTER_NUM_POINTS);

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
void FPGA::ReadDataChanenl(Chan::E ch, uint8 data[FPGA::MAX_NUM_POINTS])
{
    uint numPoints = NumPoints();

    if (addrRead == 0xffff)
    {
        addrRead = (uint16)(ReadLastRecord() - (int)numPoints);
    }
    
    FSMC::WriteToFPGA16(WR::PRED_LO, (uint16)(addrRead));
    FSMC::WriteToFPGA8(WR::START_ADDR, 0xff);


    uint8 *addr0 = Chan(ch).IsA() ? RD::DATA_A : RD::DATA_B;  // -V566
    uint8 *addr1 = addr0 + 1;

    if (IN_RANDOMIZE_MODE)
    {
        ReadDataChanenlRand(ch, addr1, data);
    }
    else
    {
        uint8 *p = data;

        *p = *addr0;    // Первая точка почему-то неправильная читается. Просто откидываем её.
        *p = *addr1;    // -V519

        if(SET_PEAKDET_EN)
        {
            for(uint i = 0; i < numPoints / 2U; i++)
            {
                *p++ = *addr0;
                *p++ = *addr1;
            }
        }
        else
        {
            for (uint i = 0; i < numPoints / 4U; ++i)   // -V112
            {
                *p++ = *addr1;
                *p++ = *addr1;
                *p++ = *addr1;
                *p++ = *addr1;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Reset()
{
    DataStorage::Init(Device::State::CurrentMode());

    TShift::Load();

    HAL::LoadRegUPR();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint FPGA::NumPoints()
{
    return (uint)(1 << (FPGA_ENUM_POINTS + 9));
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
