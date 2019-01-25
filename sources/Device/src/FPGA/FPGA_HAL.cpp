#include "defines.h"
#include "FPGA_HAL.h"
#include "Hardware/FSMC.h"
#include "Hardware/GPIO.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Settings/SettingsTime.h"
#include "Utils/Math.h"

#include "device.h"
#include "Hardware/CPU.h"


using namespace FPGA::ADDR;
using namespace FPGA::HAL::GPIO;
using namespace Osci::Settings;


extern bool givingStart;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint16 FPGA::HAL::flag = 0;

static ADC_HandleTypeDef handleADC;
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
ADC_HandleTypeDef *FPGA::HAL::_ADC::Handle()
{
    return &handleADC;
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
        HAL_GPIO_Init(GetPort((Pin::E)i), &isGPIO);
    }

    for (int i = 0; i < Pin::Number; i++)
    {
        gpio.SetOutputPP_PullDown(GetPort((Pin::E)i), (uint)::Math::LowSignedBit(GetPin((Pin::E)i)));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint FPGA::HAL::GPIO::GetPin(Pin::E pin)
{
    return pins[pin].pin;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
GPIO_TypeDef *FPGA::HAL::GPIO::GetPort(Pin::E pin)
{
    return pins[pin].gpioTD;
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
        GetPort(pin)->BSRR = GetPin(pin);
    }
    else
    {
        GetPort(pin)->BSRR = (uint)GetPin(pin) << 16;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::GPIO::SetPin(Pin::E pin)
{
    GetPort(pin)->BSRR = GetPin(pin);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::GPIO::ResetPin(Pin::E pin)
{
    GetPort(pin)->BSRR = (uint)GetPin(pin) << 16;
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::HAL::_ADC::Init()
{
    // Теперь настроим вход АЦП от рандомизатора
    // PF10 - ADC3 IN8
    // PB11 - EXTI11
    // Режим работы:
    // - измерение по 1 регулярному каналу
    // - одиночное измерение по фронту внешнего запуска (прерывание от PB11)

    __ADC3_CLK_ENABLE();

    static GPIO_InitTypeDef isGPIOadc =
    {
        GPIO_PIN_10,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOF, &isGPIOadc);

    static GPIO_InitTypeDef isGPIOexti =
    {
        GPIO_PIN_11,
        GPIO_MODE_IT_RISING,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOB, &isGPIOexti);

    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    handleADC.Instance = ADC3;
    handleADC.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    handleADC.Init.Resolution = ADC_RESOLUTION12b;
    handleADC.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handleADC.Init.ScanConvMode = DISABLE;
    handleADC.Init.EOCSelection = ENABLE;
    handleADC.Init.ContinuousConvMode = DISABLE;
    handleADC.Init.DMAContinuousRequests = DISABLE;
    handleADC.Init.NbrOfConversion = 1;
    handleADC.Init.DiscontinuousConvMode = DISABLE;
    handleADC.Init.NbrOfDiscConversion = 0;
    handleADC.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    handleADC.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_Ext_IT11;

    if (HAL_ADC_Init(&handleADC) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    ADC_ChannelConfTypeDef sConfig;
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;

    if (HAL_ADC_ConfigChannel(&handleADC, &sConfig) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if (HAL_ADC_Start_IT(&handleADC) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


extern "C" {

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EXTI1_IRQHandler()
{
    funcP2P();

    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);
}
}
