#pragma once
#include <usbh_def.h>


#define USB_CDC_CLASS                                           0x02


extern USBH_ClassTypeDef  CDC_Class;
#define USBH_CDC_CLASS    &CDC_Class


USBH_StatusTypeDef  USBH_CDC_Stop(USBH_HandleTypeDef *phost);
USBH_StatusTypeDef  USBH_CDC_Receive(USBH_HandleTypeDef *phost, uint8_t *pbuff, uint32_t length);
uint16              USBH_CDC_GetLastReceivedDataSize(USBH_HandleTypeDef *phost);
