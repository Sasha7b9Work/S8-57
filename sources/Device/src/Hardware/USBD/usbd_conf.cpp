#include "defines.h"
#include "usbd_core.h"
#include "Log.h"
#include "Display/Display.h"
#include <stm32f4xx_hal.h>
#include "Hardware/VCP.h"
#include "Settings/Settings.h"


/*******************************************************************************
LL Driver Callbacks (PCD -> USB Device Library)
*******************************************************************************/

void HAL_PCD_SetupStageCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_SetupStage(reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData), reinterpret_cast<uint8_t *>(hpcd->Setup)); //-V2571

    USBD_HandleTypeDef *pdev = reinterpret_cast<USBD_HandleTypeDef*>(hpcd->pData);  // //-V2571
    USBD_SetupReqTypedef request = pdev->request;                   //
                                                                    //
    
    static uint16 prevLength = 7;                                   //
                                                                    //
    if (request.wLength == 0)                                       //
    {                                                               //
//        if (CABLE_USB_IS_CONNECTED)                               //
        {                                                           //
            if (prevLength != 0)                                    //
            {                                                       //
                VCP::cableUSBisConnected = true;                    // Ёто потому, что при включении прибора с подключенным шнуром
                VCP::connectedToUSB = true;                         // GOVNOCODE “аким вот замысловатым образом определ€ем, что к нам подконнектилс€ хост (
            }                                                       //
            else                                                    //
            {                                                       //
                VCP::connectedToUSB = false;                        //
                //Settings::Save();                                   // ѕри отконнекчивании сохран€ем настройки
            }                                                       // \todo ¬озможно, это не нужно делать
            //CONNECTED_TO_USB = prevLength != 0;                   // 
        }                                                           //
    }                                                               //
    prevLength = request.wLength;                                   //
    
}


void HAL_PCD_DataOutStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
  USBD_LL_DataOutStage(reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData), epnum, hpcd->OUT_ep[epnum].xfer_buff); //-V2571
}


void HAL_PCD_DataInStageCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    USBD_LL_DataInStage(reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData), epnum, hpcd->IN_ep[epnum].xfer_buff); //-V2571
    
    // \todo здесь оказываем€ после каждой передачи пакета. ћожно отловить момент, когда передача закончена.
}


void HAL_PCD_SOFCallback(PCD_HandleTypeDef *hpcd)
{
  USBD_LL_SOF(reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)); //-V2571
}


void HAL_PCD_ResetCallback(PCD_HandleTypeDef *hpcd)
{ 
    USBD_SpeedTypeDef speed = USBD_SPEED_FULL;

    /* Set USB Current Speed */
    switch(hpcd->Init.speed)
    {
    case PCD_SPEED_HIGH:
        speed = USBD_SPEED_HIGH;
        break;
    
    case PCD_SPEED_FULL:
        speed = USBD_SPEED_FULL;     //-V1048
        break;
	
	default:
        speed = USBD_SPEED_FULL;     //-V1048
        break;
    }
    USBD_LL_SetSpeed(reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData), speed); //-V2571
  
    /* Reset Device */
    USBD_LL_Reset(reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)); //-V2571
}


void HAL_PCD_SuspendCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_Suspend(reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)); //-V2571
//      __HAL_PCD_GATE_PHYCLOCK(hpcd);
//  /* Enter in STOP mode. */
//  /* USER CODE BEGIN 2 */
//  if (hpcd->Init.low_power_enable)
//  {
//    /* Set SLEEPDEEP bit and SleepOnExit of Cortex System Control Register. */
//    SCB->SCR |= (uint32_t)((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));
//  }
}


void HAL_PCD_ResumeCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_Resume(reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)); //-V2571
}


void HAL_PCD_ISOOUTIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    USBD_LL_IsoOUTIncomplete(reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData), epnum); //-V2571
}


void HAL_PCD_ISOINIncompleteCallback(PCD_HandleTypeDef *hpcd, uint8_t epnum)
{
    USBD_LL_IsoINIncomplete(reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData), epnum); //-V2571
}


void HAL_PCD_ConnectCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_DevConnected(reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)); //-V2571
}


void HAL_PCD_DisconnectCallback(PCD_HandleTypeDef *hpcd)
{
    USBD_LL_DevDisconnected(reinterpret_cast<USBD_HandleTypeDef *>(hpcd->pData)); //-V2571
}


/*******************************************************************************
                       LL Driver Interface (USB Device Library --> PCD)
*******************************************************************************/

USBD_StatusTypeDef  USBD_LL_Init (USBD_HandleTypeDef *pdev)
{ 
    NVIC_SetPriority (SysTick_IRQn, 0);  
  
    if(pdev->id == VCP::DEVICE_FS)
    {
        PCD_HandleTypeDef *handlePCD = reinterpret_cast<PCD_HandleTypeDef *>(VCP::handlePCD); //-V2571
        // Link The driver to the stack
        handlePCD->pData = pdev;
        pdev->pData = handlePCD;

        handlePCD->Instance = USB_OTG_FS; //-V2571
        handlePCD->Init.dev_endpoints = 4;
        handlePCD->Init.speed = PCD_SPEED_FULL;
        handlePCD->Init.dma_enable = DISABLE;
        handlePCD->Init.ep0_mps = DEP0CTL_MPS_64;
        handlePCD->Init.phy_itface = PCD_PHY_EMBEDDED;
        handlePCD->Init.Sof_enable = DISABLE;
        handlePCD->Init.low_power_enable = DISABLE;
        handlePCD->Init.lpm_enable = DISABLE;
        handlePCD->Init.vbus_sensing_enable = ENABLE;
        handlePCD->Init.use_dedicated_ep1 = DISABLE;
    
        // Initialize LL Driver
        HAL_PCD_Init(handlePCD);
    
        HAL_PCDEx_SetRxFiFo(handlePCD, 0x80);
        HAL_PCDEx_SetTxFiFo(handlePCD, 0, 0x40);
        HAL_PCDEx_SetTxFiFo(handlePCD, 1, 0x80); 
    }

    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_DeInit(USBD_HandleTypeDef *pdev)
{
    HAL_PCD_DeInit(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData)); //-V2571
    return USBD_OK; 
}


USBD_StatusTypeDef USBD_LL_Start(USBD_HandleTypeDef *pdev)
{
    HAL_PCD_Start(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData)); //-V2571
    return USBD_OK; 
}


USBD_StatusTypeDef USBD_LL_Stop(USBD_HandleTypeDef *pdev)
{
    HAL_PCD_Stop(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData)); //-V2571
    return USBD_OK; 
}


USBD_StatusTypeDef USBD_LL_OpenEP(USBD_HandleTypeDef *pdev, 
                                  uint8_t  ep_addr,                                      
                                  uint8_t  ep_type,
                                  uint16_t ep_mps)
{
    HAL_PCD_EP_Open(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr, ep_mps, ep_type); //-V2571
    return USBD_OK; 
}


USBD_StatusTypeDef USBD_LL_CloseEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    HAL_PCD_EP_Close(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr); //-V2571
    return USBD_OK; 
}


USBD_StatusTypeDef USBD_LL_FlushEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    HAL_PCD_EP_Flush(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr); //-V2571
    return USBD_OK; 
}


USBD_StatusTypeDef USBD_LL_StallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    HAL_PCD_EP_SetStall(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr); //-V2571
    return USBD_OK; 
}


USBD_StatusTypeDef USBD_LL_ClearStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    HAL_PCD_EP_ClrStall(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr); //-V2571
    return USBD_OK; 
}


uint8_t USBD_LL_IsStallEP(USBD_HandleTypeDef *pdev, uint8_t ep_addr)   
{
    PCD_HandleTypeDef *hpcd = reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData); //-V2571
  
    if((ep_addr & 0x80) == 0x80)
    {
        return hpcd->IN_ep[ep_addr & 0x7F].is_stall; 
    }
    else
    {
        return hpcd->OUT_ep[ep_addr & 0x7F].is_stall; 
    }
}


USBD_StatusTypeDef USBD_LL_SetUSBAddress(USBD_HandleTypeDef *pdev, uint8_t dev_addr)   
{
    HAL_PCD_SetAddress(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), dev_addr); //-V2571
    return USBD_OK; 
}


USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev, 
                                    uint8_t  ep_addr,                                      
                                    uint8_t  *pbuf,
                                    uint16_t  size)
{
    HAL_PCD_EP_Transmit(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr, pbuf, size); //-V2571
    return USBD_OK;   
}


uint32_t USBD_LL_GetRxDataSize(USBD_HandleTypeDef *pdev, uint8_t  ep_addr)  
{
    return HAL_PCD_EP_GetRxCount(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr); //-V2571
}


USBD_StatusTypeDef USBD_LL_Transmit(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t *pbuf, uint32_t size)
{
    HAL_PCD_EP_Transmit(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr, pbuf, size); //-V2571
    return USBD_OK;
}


USBD_StatusTypeDef USBD_LL_PrepareReceive(USBD_HandleTypeDef *pdev, uint8_t ep_addr, uint8_t *pbuf, uint32_t size)
{
    HAL_PCD_EP_Receive(reinterpret_cast<PCD_HandleTypeDef *>(pdev->pData), ep_addr, pbuf, size); //-V2571
    return USBD_OK;
}


void  USBD_LL_Delay(uint32_t Delay)
{
    HAL_Delay(Delay);
}
