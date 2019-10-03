#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "device.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


using namespace Address;


uint16 _FPGA::flag = 0;

struct PinStruct
{
    HPort::E gpioTD;
    uint16              pin;
};

static PinStruct pins[FPin::Number] =
{
    {HPort::_C , HPin::_10},   // SPI3_SCK
    {HPort::_C , HPin::_12},   // SPI3_DAT
    {HPort::_D , HPin::_3},    // SPI3_CS1
    {HPort::_G , HPin::_13},   // SPI3_CS2
    {HPort::_D , HPin::_10},   // A1
    {HPort::_D , HPin::_11},   // A2
    {HPort::_D , HPin::_12},   // A3
    {HPort::_D , HPin::_13},   // A4
    {HPort::_G , HPin::_2},    // LF1 Ограничение полосы
    {HPort::_G , HPin::_3},    // LF2 Ограничение полосы
    {HPort::_G , HPin::_4},    // A1S
    {HPort::_G , HPin::_5},    // A0S
    {HPort::_G , HPin::_6}     // LFS
};



#define PORT(pin)   (pins[pin].gpioTD)



void _FPGA::LoadRegUPR()
{
    uint8 data = 0;

    if ((set.time.peakDet == PeakDetMode::Enabled) || Device::State::InModeRecorder())
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

    data |= mask[set.serv.calibratorMode];

    HAL_FSMC::WriteToFPGA8(WR::UPR, data);
}


void _FPGA::GPIO::Init()
{
    for (int i = 0; i < FPin::Number; i++)
    {
        HAL_PIO::Init(PORT((FPin::E)i), GetPin((FPin::E)i) , HMode::Output_PP, HPull::Down);
    }
}


uint16 _FPGA::GPIO::GetPin(FPin::E pin)
{
    return pins[pin].pin;
}


void _FPGA::GPIO::WriteRegisters(FPin::E cs, uint16 value)
{
    ResetPin(cs);

    if (cs == FPin::SPI3_CS1)
    {
        for (int i = 15; i >= 0; --i)
        {
            WritePin(FPin::SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            SetPin(FPin::SPI3_SCK);
            ResetPin(FPin::SPI3_SCK);
        }
    }
    else if (cs == FPin::SPI3_CS2)
    {
        for (int i = 0; i < 16; ++i)
        {
            WritePin(FPin::SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            SetPin(FPin::SPI3_SCK);
            ResetPin(FPin::SPI3_SCK);
        }
    }
    else
    {
        // нет действий
    }

    SetPin(cs);
}


void _FPGA::GPIO::SetPin(FPin::E pin)
{
    HAL_PIO::Set(PORT(pin), GetPin(pin));
}


void _FPGA::GPIO::ResetPin(FPin::E pin)
{
    HAL_PIO::Reset(PORT(pin), GetPin(pin));
}


void _FPGA::GPIO::WritePin(FPin::E pin, int enable)
{
    HAL_PIO::Write(PORT(pin), GetPin(pin), enable ? HState::Enabled : HState::Disabled);
}


bool _FPGA::GetFlag::DATA_READY()
{
    return _GET_BIT(flag, Flag::_DATA_READY) == 1;
}


bool _FPGA::GetFlag::TRIG_READY()
{
    return _GET_BIT(flag, Flag::_TRIG_READY) == 1;
}


bool _FPGA::GetFlag::HOLD_OFF()
{
    return _GET_BIT(flag, Flag::_HOLD_OFF_FLAG) == 1;
}


bool _FPGA::GetFlag::PRED()
{
    return _GET_BIT(flag, Flag::_PRED) == 1;
}


bool _FPGA::GetFlag::FREQ_READY()
{
    return _GET_BIT(flag, Flag::_FREQ_READY) == 1;
}


bool _FPGA::GetFlag::PERIOD_READY()
{
    return _GET_BIT(flag, Flag::_PERIOD_READY) == 1;
}


bool _FPGA::GetFlag::FREQ_OVERFLOW()
{
    return _GET_BIT(flag, Flag::_FREQ_OVERFLOW) == 1;
}


bool _FPGA::GetFlag::PERIOD_OVERFLOW()
{
    return _GET_BIT(flag, Flag::_PERIOD_OVERFLOW) == 1;
}


bool _FPGA::GetFlag::FREQ_IN_PROCESS()
{
    return _GET_BIT(flag, Flag::_FREQ_IN_PROCESS) == 1;
}


bool _FPGA::GetFlag::PERIOD_IN_PROCESS()
{
    return _GET_BIT(flag, Flag::_PERIOD_IN_PROCESS) == 1;
}


static uint8 *addrAltera = ((uint8 *)NOR_MEMORY_ADRESS1);    // -V566

namespace Address
{
    namespace WR
    {
        uint8 *START = addrAltera + 0;
        uint8 *TBASE = addrAltera + 1;
        uint8 *PRED_LO = addrAltera + 2;
        uint8 *PRED_HI = addrAltera + 3;
        uint8 *POST_LO = addrAltera + 4;
        uint8 *POST_HI = addrAltera + 5;
        uint8 *UPR = addrAltera + 6;
        uint8 *FREQMETER = addrAltera + 8;
        uint8 *TRIG = addrAltera + 9;
        uint8 *START_ADDR = addrAltera + 11;
        uint8 *RESET_COUNTER_FREQ = addrAltera + 12;
        uint8 *RESET_COUNTER_PERIOD = addrAltera + 13;
        uint8 *TRIG_HOLD_ENABLE = addrAltera + 16;
        uint8 *TRIG_HOLD_VALUE_LOW = addrAltera + 17;
        uint8 *TRIG_HOLD_VALUE_MID = addrAltera + 18;
        uint8 *TRIG_HOLD_VALUE_HI = addrAltera + 19;
    }
    namespace RD
    {
        uint8 *DATA_A = addrAltera + 0;
        uint8 *DATA_A_PEAK_MAX = DATA_A;
        uint8 *DATA_A_PEAK_MIN = DATA_A_PEAK_MAX + 1;
        uint8 *DATA_B = addrAltera + 2;
        uint8 *DATA_B_PEAK_MAX = DATA_B;
        uint8 *DATA_B_PEAK_MIN = DATA_B_PEAK_MAX + 1;
        uint8 *LAST_RECORD_LO = addrAltera + 4;
        uint8 *LAST_RECORD_HI = addrAltera + 5;
        uint8 *FREQ_BYTE_0 = addrAltera + 8;
        uint8 *FREQ_BYTE_1 = addrAltera + 9;
        uint8 *FREQ_BYTE_2 = addrAltera + 12;
        uint8 *FREQ_BYTE_3 = addrAltera + 13;
        uint8 *PERIOD_BYTE_0 = addrAltera + 16;
        uint8 *PERIOD_BYTE_1 = addrAltera + 17;
        uint8 *PERIOD_BYTE_2 = addrAltera + 20;
        uint8 *PERIOD_BYTE_3 = addrAltera + 21;
        uint8 *FLAG_LO = addrAltera + 24;
        uint8 *FLAG_HI = addrAltera + 25;
    }
}
