#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


void HAL_HCD::Init()
{
    __GPIOB_CLK_ENABLE();
    __USB_OTG_HS_CLK_ENABLE(); //-V760
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO =
    {
        GPIO_PIN_14 | GPIO_PIN_15,
        GPIO_MODE_AF_PP,
        GPIO_NOPULL,
        GPIO_SPEED_FAST,
        GPIO_AF12_OTG_HS_FS
    };

    HAL_GPIO_Init(GPIOB, &isGPIO);

    HAL_NVIC_SetPriority(OTG_HS_IRQn, 5, 1);

    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}
