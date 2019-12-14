#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>


void HAL_HCD::Init()
{
    __GPIOB_CLK_ENABLE();
    __USB_OTG_HS_CLK_ENABLE(); //-V760
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();

    HAL_PIO::Init(PIN_HCD_DM, HMode::AF_PP, HPull::No, HSpeed::High, HAlternate::AF12_OTG_HS_FS);
    HAL_PIO::Init(PIN_HCD_DP, HMode::AF_PP, HPull::No, HSpeed::High, HAlternate::AF12_OTG_HS_FS);

    HAL_NVIC_SetPriority(OTG_HS_IRQn, 5, 1);

    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}
