#pragma once
#include "stm32f4xx_ll_usb.h"


#define PCD_SPEED_FULL               2U
#define PCD_PHY_EMBEDDED             2U


typedef USB_OTG_GlobalTypeDef  PCD_TypeDef;
typedef USB_OTG_CfgTypeDef     PCD_InitTypeDef;


typedef struct
{
    int temp;

    PCD_TypeDef             *Instance;    /*!< Register base address              */
    PCD_InitTypeDef         Init;         /*!< PCD required parameters            */
//     PCD_EPTypeDef           IN_ep[16U];   /*!< IN endpoint parameters             */
//     PCD_EPTypeDef           OUT_ep[16U];  /*!< OUT endpoint parameters            */
//     HAL_LockTypeDef         Lock;         /*!< PCD peripheral status              */
//     __IO PCD_StateTypeDef   State;        /*!< PCD communication state            */
//     uint32_t                Setup[12U];   /*!< Setup packet buffer                */
// #ifdef USB_OTG_GLPMCFG_LPMEN
//     PCD_LPM_StateTypeDef    LPM_State;    /*!< LPM State                          */
//     uint32_t                BESL;
//     uint32_t                lpm_active;   /*!< Enable or disable the Link Power Management .
//                                           This parameter can be set to ENABLE or DISABLE */
// #endif /* USB_OTG_GLPMCFG_LPMEN */
// #ifdef USB_OTG_GCCFG_BCDEN
//     uint32_t battery_charging_active;     /*!< Enable or disable Battery charging.
//                                           This parameter can be set to ENABLE or DISABLE */
// #endif /* USB_OTG_GCCFG_BCDEN */
//     void                    *pData;       /*!< Pointer to upper stack Handler */
} PCD_HandleTypeDef;


HAL_StatusTypeDef HAL_PCD_Init(PCD_HandleTypeDef *hpcd);
