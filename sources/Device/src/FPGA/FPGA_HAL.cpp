#include "defines.h"
#include "FPGA_HAL.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "device.h"
#include "Hardware/HAL/HAL.h"


using namespace FPGA::HAL;
using namespace HAL::ADDRESSES::FPGA;


uint16 FPGA::HAL::flag = 0;

struct PinStruct
{
    HPort::E gpioTD;
    uint16              pin;
};

static PinStruct pins[GPIO::Pin::Number] =
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



void FPGA::HAL::LoadRegUPR()
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


void FPGA::HAL::GPIO::Init()
{
    for (int i = 0; i < Pin::Number; i++)
    {
        ::HAL::PIO::Init(PORT((Pin::E)i), GetPin((Pin::E)i) , ::HAL::PIO::Mode::Output_PP, ::HAL::PIO::Pull::Down);
    }
}


uint16 FPGA::HAL::GPIO::GetPin(Pin::E pin)
{
    return pins[pin].pin;
}


void FPGA::HAL::GPIO::WriteRegisters(Pin::E cs, uint16 value)
{
    ResetPin(cs);

    if (cs == Pin::SPI3_CS1)
    {
        for (int i = 15; i >= 0; --i)
        {
            WritePin(Pin::SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            SetPin(Pin::SPI3_SCK);
            ResetPin(Pin::SPI3_SCK);
        }
    }
    else if (cs == Pin::SPI3_CS2)
    {
        for (int i = 0; i < 16; ++i)
        {
            WritePin(Pin::SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            SetPin(Pin::SPI3_SCK);
            ResetPin(Pin::SPI3_SCK);
        }
    }
    else
    {
        // нет действий
    }

    SetPin(cs);
}


void FPGA::HAL::GPIO::SetPin(Pin::E pin)
{
    ::HAL::PIO::Set(PORT(pin), GetPin(pin));
}


void FPGA::HAL::GPIO::ResetPin(Pin::E pin)
{
    ::HAL::PIO::Reset(PORT(pin), GetPin(pin));
}


void FPGA::HAL::GPIO::WritePin(Pin::E pin, int enable)
{
    ::HAL::PIO::Write(PORT(pin), GetPin(pin), enable ? HState::Enabled : HState::Disabled);
}


bool FPGA::HAL::GetFlag::DATA_READY()
{
    return _GET_BIT(flag, Flag::_DATA_READY) == 1;
}


bool FPGA::HAL::GetFlag::TRIG_READY()
{
    return _GET_BIT(flag, Flag::_TRIG_READY) == 1;
}


bool FPGA::HAL::GetFlag::HOLD_OFF()
{
    return _GET_BIT(flag, Flag::_HOLD_OFF_FLAG) == 1;
}


bool FPGA::HAL::GetFlag::PRED()
{
    return _GET_BIT(flag, Flag::_PRED) == 1;
}


bool FPGA::HAL::GetFlag::FREQ_READY()
{
    return _GET_BIT(flag, Flag::_FREQ_READY) == 1;
}


bool FPGA::HAL::GetFlag::PERIOD_READY()
{
    return _GET_BIT(flag, Flag::_PERIOD_READY) == 1;
}


bool FPGA::HAL::GetFlag::FREQ_OVERFLOW()
{
    return _GET_BIT(flag, Flag::_FREQ_OVERFLOW) == 1;
}


bool FPGA::HAL::GetFlag::PERIOD_OVERFLOW()
{
    return _GET_BIT(flag, Flag::_PERIOD_OVERFLOW) == 1;
}


bool FPGA::HAL::GetFlag::FREQ_IN_PROCESS()
{
    return _GET_BIT(flag, Flag::_FREQ_IN_PROCESS) == 1;
}


bool FPGA::HAL::GetFlag::PERIOD_IN_PROCESS()
{
    return _GET_BIT(flag, Flag::_PERIOD_IN_PROCESS) == 1;
}
