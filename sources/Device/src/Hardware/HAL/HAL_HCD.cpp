// 2023/11/22 13:25:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>
#include <usbh_core.h>
#include <usbh_def.h>


HCD_HandleTypeDef hHCD;
void *HAL_HCD::handleHCD = &hHCD;


void HAL_HCD::Init()
{
    __GPIOB_CLK_ENABLE(); //-V2571
    __USB_OTG_HS_CLK_ENABLE(); //-V760 //-V2571
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE(); //-V2571
    __SYSCFG_CLK_ENABLE(); //-V2571

    HAL_PIO::Init(PIN_HCD_DM, HMode::AF_PP, HPull::No, HSpeed::High, HAlternate::AF12_OTG_HS_FS);
    HAL_PIO::Init(PIN_HCD_DP, HMode::AF_PP, HPull::No, HSpeed::High, HAlternate::AF12_OTG_HS_FS);

    HAL_NVIC_SetPriority(OTG_HS_IRQn, 15, 15);

    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
}


void HAL_HCD::InitUSBH_LL(USBH_HandleTypeDef *phost)
{
    hHCD.Instance = USB_OTG_HS; //-V2571
    hHCD.Init.speed = HCD_SPEED_HIGH;
    hHCD.Init.Host_channels = 12;
    hHCD.Init.dma_enable = 0;
    hHCD.Init.low_power_enable = 0;
    hHCD.Init.phy_itface = HCD_PHY_EMBEDDED;
    hHCD.Init.Sof_enable = 0;
    hHCD.Init.vbus_sensing_enable = 0;
    hHCD.Init.use_external_vbus = 0;

    hHCD.pData = phost;
    phost->pData = &hHCD;
    HAL_HCD_Init(&hHCD);
    USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&hHCD));
}
