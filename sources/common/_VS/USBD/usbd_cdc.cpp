#include "stdafx.h"
#include "usbd_def.h"
#include "usbd_cdc.h"


USBD_ClassTypeDef  USBD_CDC;

USBD_CDC_ItfTypeDef  USBD_CDC_fops;



uint8_t USBD_CDC_RegisterInterface(USBD_HandleTypeDef * /*pdev*/, USBD_CDC_ItfTypeDef * /*fops*/)
{
    return 0;
}


uint8_t USBD_CDC_SetTxBuffer(USBD_HandleTypeDef * /*pdev*/, uint8_t * /*pbuff*/, uint16_t /*length*/)
{
    return 0;
}


uint8_t USBD_CDC_TransmitPacket(USBD_HandleTypeDef * /*pdev*/)
{
    return 0;
}