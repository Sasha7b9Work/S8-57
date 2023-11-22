// 2023/11/22 13:25:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>


void HAL_PCD::Init()
{
    __GPIOA_CLK_ENABLE(); //-V2571
    __USB_OTG_FS_CLK_ENABLE(); //-V760 //-V2571
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE(); //-V2571
    __SYSCFG_CLK_ENABLE(); //-V2571

    HAL_PIO::Init(PIN_PCD_DM, HMode::AF_PP, HPull::No, HSpeed::VeryHigh, HAlternate::AF10_OTG_FS);
    HAL_PIO::Init(PIN_PCD_DP, HMode::AF_PP, HPull::No, HSpeed::VeryHigh, HAlternate::AF10_OTG_FS);
    HAL_PIO::Init(PIN_PCD_VBUS, HMode::Input, HPull::No, HSpeed::VeryHigh);

    HAL_NVIC_SetPriority(OTG_FS_IRQn, 1, 1);

    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}
