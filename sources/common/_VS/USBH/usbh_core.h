#pragma once
#include "usbh_conf.h"
#include "usbh_def.h"


#define HOST_USER_SELECT_CONFIGURATION          1
#define HOST_USER_CLASS_ACTIVE                  2
#define HOST_USER_CLASS_SELECTED                3
#define HOST_USER_CONNECTION                    4
#define HOST_USER_DISCONNECTION                 5
#define HOST_USER_UNRECOVERED_ERROR             6


USBH_StatusTypeDef  USBH_Init           (USBH_HandleTypeDef *phost, void (*pUsrFunc)(USBH_HandleTypeDef *phost, uint8_t ), uint8_t id);
USBH_StatusTypeDef  USBH_DeInit         (USBH_HandleTypeDef *phost);
USBH_StatusTypeDef  USBH_RegisterClass  (USBH_HandleTypeDef *phost, USBH_ClassTypeDef *pclass);
USBH_StatusTypeDef  USBH_Start          (USBH_HandleTypeDef *phost);
USBH_StatusTypeDef  USBH_Stop           (USBH_HandleTypeDef *phost);
USBH_StatusTypeDef  USBH_Process        (USBH_HandleTypeDef *phost);
void  USBH_LL_SetTimer(USBH_HandleTypeDef *phost, uint32_t time);
