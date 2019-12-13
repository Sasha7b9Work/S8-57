#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>


void HAL_PCD::Init()
{
    __GPIOA_CLK_ENABLE();
    __USB_OTG_FS_CLK_ENABLE(); //-V760
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();

    HAL_PIO::Init(PIN_PCD_DM, HMode::AF_PP, HPull::No, HSpeed::VeryHigh, HAlternate::AF10_OTG_FS);
    HAL_PIO::Init(PIN_PCD_DP, HMode::AF_PP, HPull::No, HSpeed::VeryHigh, HAlternate::AF10_OTG_FS);
    HAL_PIO::Init(PIN_PCD_VBUS, HMode::Input, HPull::No, HSpeed::VeryHigh);

    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);

    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}
