#include "defines.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Settings/Settings.h"
#include <stm32f4xx_hal.h>


uint16 flagFPGA = 0;


void FPGA::LoadRegUPR()
{
    uint8 data = 0;

    if (S_PEAK_DET_ENABLED || Device::InModeRecorder())
    {
        data |= 1 << BIT_UPR_PEAK;
    }

    static const uint8 mask[3] =
    {
        (1 << BIT_UPR_CALIBR_AC_DC),
        (1 << BIT_UPR_CALIBR_ZERO),
        (0)
    };

    if(Device::InModeOsci() && OSCI_IN_MODE_RANDOMIZER)
    {
        data |= 1;
    }

    data |= mask[S_SERV_CALIBRATOR_MODE];

    HAL_BUS::FPGA::Write8(WR::UPR, data);
}


void FPGA::Flag::Clear()
{
    flagFPGA = 0;
}


bool FPGA::Flag::DataReady()
{
    return _GET_BIT(flagFPGA, ::Flag::_DATA_READY) == 1;
}


bool FPGA::Flag::TrigReady()
{
    return _GET_BIT(flagFPGA, ::Flag::_TRIG_READY) == 1;
}


bool FPGA::Flag::HoldOff()
{
    return _GET_BIT(flagFPGA, ::Flag::_HOLD_OFF_FLAG) == 1;
}


bool FPGA::Flag::Pred()
{
    return _GET_BIT(flagFPGA, ::Flag::_PRED) == 1;
}


bool FPGA::Flag::FreqReady()
{
    return _GET_BIT(flagFPGA, ::Flag::_FREQ_READY) == 1;
}


bool FPGA::Flag::PeriodReady()
{
    return _GET_BIT(flagFPGA, ::Flag::_PERIOD_READY) == 1;
}


bool FPGA::Flag::FreqOverflow()
{
    return _GET_BIT(flagFPGA, ::Flag::_FREQ_OVERFLOW) == 1;
}


bool FPGA::Flag::PeriodOverflow()
{
    return _GET_BIT(flagFPGA, ::Flag::_PERIOD_OVERFLOW) == 1;
}


bool FPGA::Flag::FreqInProcess()
{
    return _GET_BIT(flagFPGA, ::Flag::_FREQ_IN_PROCESS) == 1;
}


bool FPGA::Flag::PeriodInProcess()
{
    return _GET_BIT(flagFPGA, ::Flag::_PERIOD_IN_PROCESS) == 1;
}


static uint8 *addrAltera = ((uint8 *)NOR_MEMORY_ADRESS1);    // -V566 //-V2571 //-V566

uint8 *WR::START = addrAltera + 0;
uint8 *WR::TBASE = addrAltera + 1; //-V2563
uint8 *WR::PRED_LO = addrAltera + 2; //-V2563
uint8 *WR::PRED_HI = addrAltera + 3; //-V2563
uint8 *WR::POST_LO = addrAltera + 4; //-V2563
uint8 *WR::POST_HI = addrAltera + 5; //-V2563
uint8 *WR::UPR = addrAltera + 6; //-V2563
uint8 *WR::FREQMETER = addrAltera + 8; //-V2563
uint8 *WR::TRIG = addrAltera + 9; //-V2563
uint8 *WR::START_ADDR = addrAltera + 11; //-V2563
uint8 *WR::RESET_COUNTER_FREQ = addrAltera + 12; //-V2563
uint8 *WR::RESET_COUNTER_PERIOD = addrAltera + 13; //-V2563
uint8 *WR::TRIG_HOLD_ENABLE = addrAltera + 16; //-V2563
uint8 *WR::TRIG_HOLD_VALUE_LOW = addrAltera + 17; //-V2563
uint8 *WR::TRIG_HOLD_VALUE_MID = addrAltera + 18; //-V2563
uint8 *WR::TRIG_HOLD_VALUE_HI = addrAltera + 19; //-V2563

uint8 *RD::DATA_A = addrAltera + 0;
uint8 *RD::DATA_A_PEAK_MAX = DATA_A;
uint8 *RD::DATA_A_PEAK_MIN = DATA_A_PEAK_MAX + 1;
uint8 *RD::DATA_B = addrAltera + 2; //-V2563
uint8 *RD::DATA_B_PEAK_MAX = DATA_B;
uint8 *RD::DATA_B_PEAK_MIN = DATA_B_PEAK_MAX + 1;
uint8 *RD::LAST_RECORD_LO = addrAltera + 4; //-V2563
uint8 *RD::LAST_RECORD_HI = addrAltera + 5; //-V2563
uint8 *RD::FREQ_BYTE_0 = addrAltera + 8; //-V2563
uint8 *RD::FREQ_BYTE_1 = addrAltera + 9; //-V2563
uint8 *RD::FREQ_BYTE_2 = addrAltera + 12; //-V2563
uint8 *RD::FREQ_BYTE_3 = addrAltera + 13; //-V2563
uint8 *RD::PERIOD_BYTE_0 = addrAltera + 16; //-V2563
uint8 *RD::PERIOD_BYTE_1 = addrAltera + 17; //-V2563
uint8 *RD::PERIOD_BYTE_2 = addrAltera + 20; //-V2563
uint8 *RD::PERIOD_BYTE_3 = addrAltera + 21; //-V2563
uint8 *RD::FLAG_LO = addrAltera + 24; //-V2563
uint8 *RD::FLAG_HI = addrAltera + 25; //-V2563
