#include <stm32f4xx_hal.h>
#include "defines.h"
#include "FPGA_HAL.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTime.h"
#include "Utils/Math.h"

#include "device.h"
#include "Hardware/HAL/HAL.h"


using namespace FPGA::HAL::GPIO;
using namespace HAL::ADDRESSES::FPGA;
using namespace Osci::Settings;

using HAL::FSMC;


extern bool givingStart;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16 FPGA::HAL::flag = 0;
/// Эта функция будет вызываться по приходу каждой точки
static pFuncVV funcP2P = EmptyFuncVV;


struct PinStruct
{
    GPIO_TypeDef   *gpioTD;
    uint            pin;
};

static PinStruct pins[Pin::Number] =
{
    {GPIOC, GPIO_PIN_10},   // SPI3_SCK
    {GPIOC, GPIO_PIN_12},   // SPI3_DAT
    {GPIOD, GPIO_PIN_3},    // SPI3_CS1
    {GPIOG, GPIO_PIN_13},   // SPI3_CS2
    {GPIOD, GPIO_PIN_10},   // A1
    {GPIOD, GPIO_PIN_11},   // A2
    {GPIOD, GPIO_PIN_12},   // A3
    {GPIOD, GPIO_PIN_13},   // A4
    {GPIOG, GPIO_PIN_2},    // LF1
    {GPIOG, GPIO_PIN_3},    // LF2
    {GPIOG, GPIO_PIN_4},    // A1S
    {GPIOG, GPIO_PIN_5},    // A0S
    {GPIOG, GPIO_PIN_6}     // LFS
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PORT(pin)   (pins[pin].gpioTD)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::HAL::LoadRegUPR()
{
    uint8 data = 0;

    if (SET_PEAKDET_EN || Device::State::InModeRecorder())
    {
        data |= 1 << BIT_UPR_PEAK;
    }

    static const uint8 mask[3] =
    {
        (1 << BIT_UPR_CALIBR_AC_DC),
        (1 << BIT_UPR_CALIBR_ZERO),
        (0)
    };

    data |= mask[CALIBRATOR_MODE];

    FSMC::WriteToFPGA8(WR::UPR, data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::GPIO::Init()
{
    GPIO_InitTypeDef isGPIO =
    {
        0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLDOWN
    };

    for (int i = 0; i < Pin::Number; i++)
    {
        isGPIO.Pin = GetPin((Pin::E)i);
        HAL_GPIO_Init(PORT((Pin::E)i), &isGPIO);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint FPGA::HAL::GPIO::GetPin(Pin::E pin)
{
    return pins[pin].pin;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::GPIO::WritePin(Pin::E pin, int enable)
{
    if (enable)
    {
        PORT(pin)->BSRR = GetPin(pin);
    }
    else
    {
        PORT(pin)->BSRR = (uint)GetPin(pin) << 16;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::GPIO::SetPin(Pin::E pin)
{
    PORT(pin)->BSRR = GetPin(pin);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::GPIO::ResetPin(Pin::E pin)
{
    PORT(pin)->BSRR = (uint)GetPin(pin) << 16;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::ReadFlag()
{
    flag = (uint16)(FSMC::ReadFromFPGA(RD::FLAG_LO) | (FSMC::ReadFromFPGA(RD::FLAG_HI) << 8));

    if (GetFlag::TRIG_READY() && !givingStart)
    {
        Trig::pulse = true;
    }

    FrequencyCounter::Update();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::HAL::GetFlag::DATA_READY()
{
    return _GET_BIT(flag, Flag::_DATA_READY) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::HAL::GetFlag::TRIG_READY()
{
    return _GET_BIT(flag, Flag::_TRIG_READY) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::HAL::GetFlag::HOLD_OFF()
{
    return _GET_BIT(flag, Flag::_HOLD_OFF_FLAG) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::HAL::GetFlag::PRED()
{
    return _GET_BIT(flag, Flag::_PRED) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::HAL::GetFlag::FREQ_READY()
{
    return _GET_BIT(flag, Flag::_FREQ_READY) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::HAL::GetFlag::PERIOD_READY()
{
    return _GET_BIT(flag, Flag::_PERIOD_READY) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::HAL::GetFlag::FREQ_OVERFLOW()
{
    return _GET_BIT(flag, Flag::_FREQ_OVERFLOW) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::HAL::GetFlag::PERIOD_OVERFLOW()
{
    return _GET_BIT(flag, Flag::_PERIOD_OVERFLOW) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::HAL::GetFlag::FREQ_IN_PROCESS()
{
    return _GET_BIT(flag, Flag::_FREQ_IN_PROCESS) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::HAL::GetFlag::PERIOD_IN_PROCESS()
{
    return _GET_BIT(flag, Flag::_PERIOD_IN_PROCESS) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::Interrupt::P2P::Init(pFuncVV func)
{
    funcP2P = func;

    static GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_1,
        GPIO_MODE_IT_RISING,
        GPIO_NOPULL
    };

    HAL_GPIO_Init(GPIOG, &isGPIO);

    HAL_NVIC_SetPriority(EXTI1_IRQn, 10, 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::Interrupt::P2P::Enable()
{
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::Interrupt::P2P::Disable()
{
    HAL_NVIC_DisableIRQ(EXTI1_IRQn);
}

INTERRUPT_BEGIN

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EXTI1_IRQHandler()
{
    funcP2P();

    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
}

INTERRUPT_END
