#include "defines.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "usbh_core.h"
#include <stm32f4xx_hal.h>


void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_IncTimer (reinterpret_cast<USBH_HandleTypeDef *>(hhcd->pData)); //-V2571
}


void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_Connect(reinterpret_cast<USBH_HandleTypeDef *>(hhcd->pData)); //-V2571
}

void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_Disconnect(reinterpret_cast<USBH_HandleTypeDef *>(hhcd->pData)); //-V2571
} 

void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *, uint8_t, HCD_URBStateTypeDef)
{
    /* To be used with OS to sync URB state with the global state machine */
}

USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef *phost)
{
    HAL_HCD::InitUSBH_LL(phost);

    return USBH_OK;
}

USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef *phost)
{
    HAL_HCD_DeInit(reinterpret_cast<HCD_HandleTypeDef *>(phost->pData)); //-V2571
    return USBH_OK; 
}

USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef *phost)
{
    HAL_HCD_Start(reinterpret_cast<HCD_HandleTypeDef *>(phost->pData)); //-V2571
    return USBH_OK; 
}

USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef *phost)
{
    HAL_HCD_Stop(reinterpret_cast<HCD_HandleTypeDef *>(phost->pData)); //-V2571
    return USBH_OK; 
}

USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef *phost)
{
    USBH_SpeedTypeDef speed = USBH_SPEED_FULL;

    switch (HAL_HCD_GetCurrentSpeed(reinterpret_cast<HCD_HandleTypeDef *>(phost->pData))) //-V2571
    {
    case 0:
        speed = USBH_SPEED_HIGH;
        break;

    case 1:
        speed = USBH_SPEED_FULL; //-V1048
        break;

    case 2:
        speed = USBH_SPEED_LOW;
        break;

    default:
        speed = USBH_SPEED_FULL; //-V1048
        break;
    }
    return speed;
}


USBH_StatusTypeDef USBH_LL_ResetPort(USBH_HandleTypeDef *phost)
{
    HAL_HCD_ResetPort(reinterpret_cast<HCD_HandleTypeDef *>(phost->pData)); //-V2571
    return USBH_OK;
}


uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    return HAL_HCD_HC_GetXferCount(reinterpret_cast<HCD_HandleTypeDef *>(phost->pData), pipe); //-V2571
}


USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t epnum, uint8_t dev_address, uint8_t speed, uint8_t ep_type, uint16_t mps)
{
    HAL_HCD_HC_Init(reinterpret_cast<HCD_HandleTypeDef *>(phost->pData), pipe, epnum, dev_address, speed, ep_type, mps); //-V2571
    return USBH_OK;
}


USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    HAL_HCD_HC_Halt(reinterpret_cast<HCD_HandleTypeDef *>(phost->pData), pipe); //-V2571
    return USBH_OK;
}


USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t direction, uint8_t ep_type, uint8_t token, uint8_t *pbuff, uint16_t length, uint8_t do_ping)
{
    HAL_HCD_HC_SubmitRequest(reinterpret_cast<HCD_HandleTypeDef *>(phost->pData), pipe, direction, ep_type, token, pbuff, length, do_ping); //-V2571
    return USBH_OK;
}


USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    return static_cast<USBH_URBStateTypeDef>(HAL_HCD_HC_GetURBState(reinterpret_cast<HCD_HandleTypeDef *>(phost->pData), pipe)); //-V2571
}


USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef *, uint8_t)
{
/*
    if(state == 0)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
    }
*/

    HAL_Delay(200);
    return USBH_OK;
}


USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef *, uint8_t pipe, uint8_t toggle)   
{
    if(reinterpret_cast<HCD_HandleTypeDef *>(HAL_HCD::handleHCD)->hc[pipe].ep_is_in) //-V2571
    {
        reinterpret_cast<HCD_HandleTypeDef *>(HAL_HCD::handleHCD)->hc[pipe].toggle_in = toggle; //-V2571
    }
    else
    {
        reinterpret_cast<HCD_HandleTypeDef *>(HAL_HCD::handleHCD)->hc[pipe].toggle_out = toggle; //-V2571
    }
    return USBH_OK; 
}


uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef *, uint8_t pipe)   
{
    uint8_t toggle = 0;
  
    if(reinterpret_cast<HCD_HandleTypeDef *>(HAL_HCD::handleHCD)->hc[pipe].ep_is_in) //-V2571
    {
        toggle = reinterpret_cast<HCD_HandleTypeDef *>(HAL_HCD::handleHCD)->hc[pipe].toggle_in; //-V2571
    }
    else
    {
        toggle = reinterpret_cast<HCD_HandleTypeDef *>(HAL_HCD::handleHCD)->hc[pipe].toggle_out; //-V2571
    }
    return toggle; 
}


void USBH_Delay(uint32_t Delay)
{
    HAL_Delay(Delay);  
}


void HAL_HCD_PortEnabled_Callback(HCD_HandleTypeDef *)
{
    //USBH_LL_PortEnabled((USBH_HandleTypeDef *)hhcd->pData);
}


void HAL_HCD_PortDisabled_Callback(HCD_HandleTypeDef *)
{
    //USBH_LL_PortDisabled((USBH_HandleTypeDef *)hhcd->pData);
}
