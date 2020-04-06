#include <stm32f4xx_hal.h>
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"



static ADC_HandleTypeDef handle;


// Сконфигурировать на чтение по прерыванию
static void ConfigToIT();

// Сконфигурировать на чтение в ручном режиме
static void ConfigToHand();


void HAL_ADC3::Init()
{
    // Теперь настроим вход АЦП от рандомизатора
    // PF10 - ADC3 IN8
    // PB11 - EXTI11
    // Режим работы:
    // - измерение по 1 регулярному каналу
    // - одиночное измерение по фронту внешнего запуска (прерывание от PB11)

    __ADC3_CLK_ENABLE();

    HAL_PIO::Init(PIN_ADC3, HMode::Analog, HPull::No);

    HAL_PIO::Init(PIN_ADC3_IT, HMode::RisingIT, HPull::No);

    HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    ConfigToIT();
}


static void ConfigToIT()
{
    handle.Instance = ADC3;
    handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    handle.Init.Resolution = ADC_RESOLUTION12b;
    handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handle.Init.ScanConvMode = DISABLE;
    handle.Init.EOCSelection = ENABLE;
    handle.Init.ContinuousConvMode = DISABLE;
    handle.Init.DMAContinuousRequests = DISABLE;
    handle.Init.NbrOfConversion = 1;
    handle.Init.DiscontinuousConvMode = DISABLE;
    handle.Init.NbrOfDiscConversion = 0;
    handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    handle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_Ext_IT11;

    HAL_ADC_Init(&handle);

    ADC_ChannelConfTypeDef sConfig;
    sConfig.Channel = ADC_CHANNEL_8;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
    sConfig.Offset = 0;

    HAL_ADC_ConfigChannel(&handle, &sConfig);

    HAL_ADC_Start_IT(&handle);
}


static void ConfigToHand()
{

}


INTERRUPT_BEGIN


void ADC_IRQHandler(void)
{
    HAL_ADC_IRQHandler(&handle);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    /// \todo временная затычка. Не в рандомизаторе эта функция вообще не должна вызываться

    if (OSCI_IN_MODE_RANDOMIZER)
    {
        ConfigToHand();

        Osci::valueADC = static_cast<uint16>(HAL_ADC_GetValue(hadc));

        ConfigToIT();
    }
}

INTERRUPT_END
