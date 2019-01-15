#include "defines.h"
#include "log.h"
#include "device.h"
#include <stm32f4xx.h>
#include "FPGA.h"
#include "FPGA_HAL.h"
#include "FPGA_Settings.h"
#include "AD9286.h"
#include "Data/DataStorage.h"
#include "Display/Display.h"
#include "Hardware/CPU.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include "Utils/Values.h"
#include <stdlib.h>


using namespace FPGA::HAL::GPIO;

extern bool givingStart;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::Init()
{
    givingStart = false;

    HAL::GPIO::Init();

    SetPin(Pin::SPI3_CS1);
    SetPin(Pin::SPI3_CS2);

    ResetPin(Pin::SPI3_SCK);
    ResetPin(Pin::SPI3_DAT);

    AD9286::Init();

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

    ADC_HandleTypeDef &handleADC = *FPGA::HAL::HandleADC();

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

    DataStorage::Init(Device::CurrentMode());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadData()
{
    Buffer dataA(FPGA::NumPoints());    // -V656
    Buffer dataB(FPGA::NumPoints());    // -V656

    ReadDataChanenl(Chan::A, dataA.data);
    ReadDataChanenl(Chan::B, dataB.data);

    DataSettings ds;
    ds.Fill(dataA.data, dataB.data);

    DataStorage::Push(&ds);
}
