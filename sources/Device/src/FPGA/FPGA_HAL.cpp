#include "defines.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Settings/Settings.h"
#include <stm32f4xx_hal.h>


FlagFPGA FPGA::flag;

uint16 FlagFPGA::flag = 0;


void FPGA::LoadRegUPR()
{
    uint8 data = 0;

    if (PeakDetMode().IsEnabled() || Device::InModeRecorder())
    {
        data |= 1 << BIT_UPR_PEAK;
    }

    static const uint8 mask[3] =
    {
        (1 << BIT_UPR_CALIBR_AC_DC),
        (1 << BIT_UPR_CALIBR_ZERO),
        (0)
    };

    data |= 1;

    data |= mask[Calibrator::Mode()];

    HAL_FSMC::WriteToFPGA8(WR::UPR, data);
}


void GPIO::Init()
{
    HAL_PIO::Init(PIN_SPI3_SCK, HMode::Output_PP, HPull::Down);     // SPI3_SCK
    HAL_PIO::Init(PIN_SPI3_DAT, HMode::Output_PP, HPull::Down);     // SPI3_DAT
    HAL_PIO::Init(PIN_SPI3_CS1, HMode::Output_PP, HPull::Down);     // SPI3_CS1
    HAL_PIO::Init(PIN_SPI3_CS2, HMode::Output_PP, HPull::Down);     // SPI3_CS2
    HAL_PIO::Init(PIN_A1, HMode::Output_PP, HPull::Down);           // A1
    HAL_PIO::Init(PIN_A2, HMode::Output_PP, HPull::Down);           // A2
    HAL_PIO::Init(PIN_A3, HMode::Output_PP, HPull::Down);           // A3
    HAL_PIO::Init(PIN_A4, HMode::Output_PP, HPull::Down);           // A4
    HAL_PIO::Init(PIN_LF1, HMode::Output_PP, HPull::Down);          // LF1 Ограничение полосы
    HAL_PIO::Init(PIN_LF2, HMode::Output_PP, HPull::Down);          // LF2 Ограничение полосы
    HAL_PIO::Init(PIN_LF3, HMode::Output_PP, HPull::Down);          // A1S
    HAL_PIO::Init(PIN_A0S, HMode::Output_PP, HPull::Down);          // A0S
    HAL_PIO::Init(PIN_LFS, HMode::Output_PP, HPull::Down);          // LFS
}


void GPIO::WriteRegisters(HPort::E portCS, uint16 pinCS, uint16 value)
{
    HAL_PIO::Reset(portCS, pinCS);

    if(portCS == PORT_SPI3_CS1)
    {
        for (int i = 15; i >= 0; --i)
        {
            HAL_PIO::Write(PIN_SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            HAL_PIO::Set(PIN_SPI3_SCK);
            HAL_PIO::Reset(PIN_SPI3_SCK);
        }
    }
    else if (portCS == PORT_SPI3_CS2)
    {
        for (int i = 0; i < 16; ++i)
        {
            HAL_PIO::Write(PIN_SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            HAL_PIO::Set(PIN_SPI3_SCK);
            HAL_PIO::Reset(PIN_SPI3_SCK);
        }
    }
    else
    {
        // нет действий
    }

    HAL_PIO::Reset(portCS, pinCS);
}


bool FlagFPGA::DataReady()
{
    return _GET_BIT(flag, Flag::_DATA_READY) == 1;
}


bool FlagFPGA::TrigReady()
{
    return _GET_BIT(flag, Flag::_TRIG_READY) == 1;
}


bool FlagFPGA::HoldOff()
{
    return _GET_BIT(flag, Flag::_HOLD_OFF_FLAG) == 1;
}


bool FlagFPGA::Pred()
{
    return _GET_BIT(flag, Flag::_PRED) == 1;
}


bool FlagFPGA::FreqReady()
{
    return _GET_BIT(flag, Flag::_FREQ_READY) == 1;
}


bool FlagFPGA::PeriodReady()
{
    return _GET_BIT(flag, Flag::_PERIOD_READY) == 1;
}


bool FlagFPGA::FreqOverflow()
{
    return _GET_BIT(flag, Flag::_FREQ_OVERFLOW) == 1;
}


bool FlagFPGA::PeriodOverflow()
{
    return _GET_BIT(flag, Flag::_PERIOD_OVERFLOW) == 1;
}


bool FlagFPGA::FreqInProcess()
{
    return _GET_BIT(flag, Flag::_FREQ_IN_PROCESS) == 1;
}


bool FlagFPGA::PeriodInProcess()
{
    return _GET_BIT(flag, Flag::_PERIOD_IN_PROCESS) == 1;
}


static uint8 *addrAltera = ((uint8 *)NOR_MEMORY_ADRESS1);    // -V566

uint8 *WR::START = addrAltera + 0;
uint8 *WR::TBASE = addrAltera + 1;
uint8 *WR::PRED_LO = addrAltera + 2;
uint8 *WR::PRED_HI = addrAltera + 3;
uint8 *WR::POST_LO = addrAltera + 4;
uint8 *WR::POST_HI = addrAltera + 5;
uint8 *WR::UPR = addrAltera + 6;
uint8 *WR::FREQMETER = addrAltera + 8;
uint8 *WR::TRIG = addrAltera + 9;
uint8 *WR::START_ADDR = addrAltera + 11;
uint8 *WR::RESET_COUNTER_FREQ = addrAltera + 12;
uint8 *WR::RESET_COUNTER_PERIOD = addrAltera + 13;
uint8 *WR::TRIG_HOLD_ENABLE = addrAltera + 16;
uint8 *WR::TRIG_HOLD_VALUE_LOW = addrAltera + 17;
uint8 *WR::TRIG_HOLD_VALUE_MID = addrAltera + 18;
uint8 *WR::TRIG_HOLD_VALUE_HI = addrAltera + 19;

uint8 *RD::DATA_A = addrAltera + 0;
uint8 *RD::DATA_A_PEAK_MAX = DATA_A;
uint8 *RD::DATA_A_PEAK_MIN = DATA_A_PEAK_MAX + 1;
uint8 *RD::DATA_B = addrAltera + 2;
uint8 *RD::DATA_B_PEAK_MAX = DATA_B;
uint8 *RD::DATA_B_PEAK_MIN = DATA_B_PEAK_MAX + 1;
uint8 *RD::LAST_RECORD_LO = addrAltera + 4;
uint8 *RD::LAST_RECORD_HI = addrAltera + 5;
uint8 *RD::FREQ_BYTE_0 = addrAltera + 8;
uint8 *RD::FREQ_BYTE_1 = addrAltera + 9;
uint8 *RD::FREQ_BYTE_2 = addrAltera + 12;
uint8 *RD::FREQ_BYTE_3 = addrAltera + 13;
uint8 *RD::PERIOD_BYTE_0 = addrAltera + 16;
uint8 *RD::PERIOD_BYTE_1 = addrAltera + 17;
uint8 *RD::PERIOD_BYTE_2 = addrAltera + 20;
uint8 *RD::PERIOD_BYTE_3 = addrAltera + 21;
uint8 *RD::FLAG_LO = addrAltera + 24;
uint8 *RD::FLAG_HI = addrAltera + 25;
