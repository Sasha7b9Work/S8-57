#include "defines.h"
#include "HAL.h"
#include "Tester/Tester.h"
#include <stm32f4xx_hal.h>


namespace HAL_DAC2
{
    static DAC_HandleTypeDef handle = { DAC }; //-V2571
}


void HAL_DAC2::Init()
{
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;      // Включаем ЦАП //-V2571

    if (HAL_DAC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);

    HAL_PIO::Init(PIN_TESTER_DAC, HMode::Analog, HPull::No);

    if (HAL_DAC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    DAC_ChannelConfTypeDef configDAC;
    configDAC.DAC_Trigger = DAC_TRIGGER_NONE;
    configDAC.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;

    if (HAL_DAC_ConfigChannel(&handle, &configDAC, DAC1_CHANNEL_2) != HAL_OK)
    {
        ERROR_HANDLER();
    }
    HAL_DAC_SetValue(&handle, DAC1_CHANNEL_2, DAC_ALIGN_8B_R, 0);
    HAL_DAC_Start(&handle, DAC1_CHANNEL_2);
}


void HAL_DAC2::SetValue(uint value)
{
    HAL_DAC_SetValue(&handle, DAC1_CHANNEL_2, DAC_ALIGN_8B_R, value);
}
