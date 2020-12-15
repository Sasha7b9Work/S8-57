#pragma once
#include <stm32f4xx_hal_pcd.h>


struct USBD_HandleTypeDef
{
    void *pClassData;
};

typedef enum {
    USBD_OK = 0,
    USBD_BUSY,
    USBD_FAIL,
}USBD_StatusTypeDef;


typedef struct
{
    int i;
//    uint8_t  *(*GetDeviceDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
//    uint8_t  *(*GetLangIDStrDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
//    uint8_t  *(*GetManufacturerStrDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
//    uint8_t  *(*GetProductStrDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
//    uint8_t  *(*GetSerialStrDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
//    uint8_t  *(*GetConfigurationStrDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
//    uint8_t  *(*GetInterfaceStrDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
//#if (USBD_LPM_ENABLED == 1)
//    uint8_t  *(*GetBOSDescriptor)(USBD_SpeedTypeDef speed, uint16_t *length);
//#endif  
} USBD_DescriptorsTypeDef;

typedef struct _Device_cb //-V2573
{
    int i;
//    uint8_t(*Init)             (struct _USBD_HandleTypeDef *pdev, uint8_t cfgidx);
//    uint8_t(*DeInit)           (struct _USBD_HandleTypeDef *pdev, uint8_t cfgidx);
//    /* Control Endpoints*/
//    uint8_t(*Setup)            (struct _USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef  *req);
//    uint8_t(*EP0_TxSent)       (struct _USBD_HandleTypeDef *pdev);
//    uint8_t(*EP0_RxReady)      (struct _USBD_HandleTypeDef *pdev);
//    /* Class Specific Endpoints*/
//    uint8_t(*DataIn)           (struct _USBD_HandleTypeDef *pdev, uint8_t epnum);
//    uint8_t(*DataOut)          (struct _USBD_HandleTypeDef *pdev, uint8_t epnum);
//    uint8_t(*SOF)              (struct _USBD_HandleTypeDef *pdev);
//    uint8_t(*IsoINIncomplete)  (struct _USBD_HandleTypeDef *pdev, uint8_t epnum);
//    uint8_t(*IsoOUTIncomplete) (struct _USBD_HandleTypeDef *pdev, uint8_t epnum);
//
//    uint8_t  *(*GetHSConfigDescriptor)(uint16_t *length);
//    uint8_t  *(*GetFSConfigDescriptor)(uint16_t *length);
//    uint8_t  *(*GetOtherSpeedConfigDescriptor)(uint16_t *length);
//    uint8_t  *(*GetDeviceQualifierDescriptor)(uint16_t *length);
//#if (USBD_SUPPORT_USER_STRING == 1)
//    uint8_t  *(*GetUsrStrDescriptor)(struct _USBD_HandleTypeDef *pdev, uint8_t index, uint16_t *length);
//#endif  
} USBD_ClassTypeDef;