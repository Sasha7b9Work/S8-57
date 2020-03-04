#include "defines.h"
#include <usbh_def.h>



USBH_StatusTypeDef  USBH_Process(USBH_HandleTypeDef * /*phost*/)
{
    return USBH_OK;
}


USBH_StatusTypeDef  USBH_Init(USBH_HandleTypeDef * /*phost*/, void(* /*pUsrFunc*/)(USBH_HandleTypeDef *phost, uint8_t), uint8_t /*id*/)
{
    return USBH_OK;
}


USBH_StatusTypeDef USBH_DeInit(USBH_HandleTypeDef * /*phost*/)
{
    return USBH_OK;
}


USBH_StatusTypeDef  USBH_RegisterClass(USBH_HandleTypeDef * /*phost*/, USBH_ClassTypeDef * /*pclass*/)
{
    return USBH_OK;
}


USBH_StatusTypeDef  USBH_Start(USBH_HandleTypeDef * /*phost*/)
{
    return USBH_OK;
}


USBH_StatusTypeDef USBH_Stop(USBH_HandleTypeDef * /*phost*/)
{
    return USBH_OK;
}

void  USBH_LL_SetTimer(USBH_HandleTypeDef *phost, uint32_t time)
{
    phost->Timer = time;
}
