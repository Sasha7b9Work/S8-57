#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>
#include <usbh_core.h>
#include <usbh_def.h>


HCD_HandleTypeDef handleHCD;

HCD_HandleTypeDef  &HAL_HCD::handle = handleHCD;


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


void HAL_HCD::InitUSBH_LL(USBH_HandleTypeDef *phost)
{
    handle.Instance = USB_OTG_HS;
    handle.Init.speed = HCD_SPEED_HIGH;
    handle.Init.Host_channels = 12;
    handle.Init.dma_enable = 0;
    handle.Init.low_power_enable = 0;
    handle.Init.phy_itface = HCD_PHY_EMBEDDED;
    handle.Init.Sof_enable = 0;
    handle.Init.vbus_sensing_enable = 0;
    handle.Init.use_external_vbus = 0;

    handle.pData = phost;
    phost->pData = &handleHCD;
    HAL_HCD_Init(&handle);
    USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&handle));
}
