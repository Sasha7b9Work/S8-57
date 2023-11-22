// 2023/11/22 13:35:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"
#include <stm32f4xx_hal.h>


namespace HAL_ADC3
{
    static ADC_HandleTypeDef handle;

    static ADC_ChannelConfTypeDef config =
    {
        ADC_CHANNEL_8,
        1,
        ADC_SAMPLETIME_28CYCLES,
        0
    };


    // Сконфигурировать на чтение по прерыванию
    static void ConfigToIT();

    // Сконфигурировать на чтение в ручном режиме
    static void ConfigToHand();

    static uint16 value = 0;
}


void HAL_ADC3::Init()
{
    // Теперь настроим вход АЦП от рандомизатора
    // PF10 - ADC3 IN8
    // PB11 - EXTI11
    // Режим работы:
    // - измерение по 1 регулярному каналу
    // - одиночное измерение по фронту внешнего запуска (прерывание от PB11)

    __ADC3_CLK_ENABLE(); //-V2571

    HAL_PIO::Init(PIN_ADC3, HMode::Analog, HPull::No);

    HAL_PIO::Init(PIN_ADC3_IT, HMode::RisingIT, HPull::No);

    HAL_NVIC_SetPriority(ADC_IRQn, 1, 1);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    handle.Instance = ADC3; //-V2571
    handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;
    handle.Init.Resolution = ADC_RESOLUTION12b;
    handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    handle.Init.ScanConvMode = DISABLE;
    handle.Init.ContinuousConvMode = DISABLE;
    handle.Init.DMAContinuousRequests = DISABLE;
    handle.Init.NbrOfConversion = 1;
    handle.Init.DiscontinuousConvMode = DISABLE;
    handle.Init.NbrOfDiscConversion = 0;

    ConfigToIT();
}


uint16 HAL_ADC3::ValueRandomizer()
{
    return value;
}


void HAL_ADC3::ConfigToIT()
{
    handle.Init.EOCSelection = ENABLE;
    handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
    handle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_Ext_IT11;

    HAL_ADC_Init(&handle);

    HAL_ADC_ConfigChannel(&handle, &config);

    HAL_ADC_Start_IT(&handle);
}


void HAL_ADC3::ConfigToHand()
{
    handle.Init.EOCSelection = DISABLE;
    handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    handle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;

    HAL_ADC_Init(&handle);

    HAL_ADC_ConfigChannel(&handle, &config);

    HAL_ADC_Start(&handle);
}


INTERRUPT_BEGIN


void ADC_IRQHandler(void)
{
    HAL_ADC_IRQHandler(&HAL_ADC3::handle);
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    // \todo временная затычка. Не в рандомизаторе эта функция вообще не должна вызываться

    uint start = TIME_US;

    if (OSCI_IN_MODE_RANDOMIZER)
    {
        HAL_ADC3::ConfigToHand();

        while(TIME_US - start < 5)
        {
        }

        HAL_ADC_PollForConversion(&HAL_ADC3::handle, 10);

        HAL_ADC3::value = static_cast<uint16>(HAL_ADC_GetValue(hadc));

        HAL_ADC3::ConfigToIT();
    }
}

INTERRUPT_END
