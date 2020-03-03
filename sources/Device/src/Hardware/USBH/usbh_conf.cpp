#include "defines.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "usbh_core.h"


void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_IncTimer ((USBH_HandleTypeDef *)hhcd->pData);
}


void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_Connect((USBH_HandleTypeDef *)hhcd->pData);
}

void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_Disconnect((USBH_HandleTypeDef *)hhcd->pData);
} 

void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *, uint8_t, HCD_URBStateTypeDef)
{
    /* To be used with OS to sync URB state with the global state machine */
}

USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef *phost)
{  
    FDrive::handleHCD.Instance = USB_OTG_HS;
    FDrive::handleHCD.Init.speed = HCD_SPEED_HIGH;
    FDrive::handleHCD.Init.Host_channels = 12; 
    FDrive::handleHCD.Init.dma_enable = 0;
    FDrive::handleHCD.Init.low_power_enable = 0;
    FDrive::handleHCD.Init.phy_itface = HCD_PHY_EMBEDDED; 
    FDrive::handleHCD.Init.Sof_enable = 0;
    FDrive::handleHCD.Init.vbus_sensing_enable = 0;
    FDrive::handleHCD.Init.use_external_vbus = 0;  

    FDrive::handleHCD.pData = phost;
    phost->pData = &FDrive::handleHCD;
    HAL_HCD_Init(&FDrive::handleHCD);
    USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&FDrive::handleHCD));
  
    return USBH_OK;
}

USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef *phost)
{
    HAL_HCD_DeInit((HCD_HandleTypeDef *)phost->pData);
    return USBH_OK; 
}

USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef *phost)
{
    HAL_HCD_Start((HCD_HandleTypeDef *)phost->pData);
    return USBH_OK; 
}

USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef *phost)
{
    HAL_HCD_Stop((HCD_HandleTypeDef *)phost->pData);
    return USBH_OK; 
}

USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef *phost)
{
    USBH_SpeedTypeDef speed = USBH_SPEED_FULL;

    switch (HAL_HCD_GetCurrentSpeed((HCD_HandleTypeDef *)phost->pData))
    {
    case 0:
        speed = USBH_SPEED_HIGH;
        break;

    case 1:
        speed = USBH_SPEED_FULL;
        break;

    case 2:
        speed = USBH_SPEED_LOW;
        break;

    default:
        speed = USBH_SPEED_FULL;
        break;
    }
    return speed;
}


USBH_StatusTypeDef USBH_LL_ResetPort(USBH_HandleTypeDef *phost)
{
    HAL_HCD_ResetPort((HCD_HandleTypeDef *)phost->pData);
    return USBH_OK;
}


uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    return HAL_HCD_HC_GetXferCount((HCD_HandleTypeDef *)phost->pData, pipe);
}


USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t epnum, uint8_t dev_address, uint8_t speed, uint8_t ep_type, uint16_t mps)
{
    HAL_HCD_HC_Init((HCD_HandleTypeDef *)phost->pData, pipe, epnum, dev_address, speed, ep_type, mps);
    return USBH_OK;
}


USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    HAL_HCD_HC_Halt((HCD_HandleTypeDef *)phost->pData, pipe);
    return USBH_OK;
}


USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t direction, uint8_t ep_type, uint8_t token, uint8_t *pbuff, uint16_t length, uint8_t do_ping)
{
    HAL_HCD_HC_SubmitRequest((HCD_HandleTypeDef *)phost->pData, pipe, direction, ep_type, token, pbuff, length, do_ping);
    return USBH_OK;
}


USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    return (USBH_URBStateTypeDef)HAL_HCD_HC_GetURBState((HCD_HandleTypeDef *)phost->pData, pipe);
}


USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef *, uint8_t)
{
    Timer::PauseOnTime(200);
    return USBH_OK;
}


USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef *, uint8_t pipe, uint8_t toggle)   
{
    if(FDrive::handleHCD.hc[pipe].ep_is_in)
    {
        FDrive::handleHCD.hc[pipe].toggle_in = toggle;
    }
    else
    {
        FDrive::handleHCD.hc[pipe].toggle_out = toggle;
    }
    return USBH_OK; 
}


uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef *, uint8_t pipe)   
{
    uint8_t toggle = 0;
  
    if(FDrive::handleHCD.hc[pipe].ep_is_in)
    {
        toggle = FDrive::handleHCD.hc[pipe].toggle_in;
    }
    else
    {
        toggle = FDrive::handleHCD.hc[pipe].toggle_out;
    }
    return toggle; 
}


void USBH_Delay(uint32_t Delay)
{
    Timer::PauseOnTime(Delay);  
}

#if (USBH_DEBUG_LEVEL > 1)

/*
void USBH_UsrLog(char *format, ...) {
}

void USBH_ErrLog(char *format, ...) {
}
void USBH_DbgLog(char *format, ...) {
}
*/
#endif
