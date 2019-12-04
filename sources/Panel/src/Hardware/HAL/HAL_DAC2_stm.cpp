#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


static DAC_HandleTypeDef handle = { DAC };


void HAL_DAC2::Init()
{
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;      // Включаем ЦАП

    if (HAL_DAC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);

    // Инициализируем ЦАП
    GPIO_InitTypeDef _gpio =
    {
        GPIO_PIN_5,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOA, &_gpio);

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

    HAL_DAC_Start(&handle, DAC1_CHANNEL_2);
}


void HAL_DAC2::SetValue(uint value)
{
    HAL_DAC_SetValue(&handle, DAC1_CHANNEL_2, DAC_ALIGN_8B_R, value);
}
