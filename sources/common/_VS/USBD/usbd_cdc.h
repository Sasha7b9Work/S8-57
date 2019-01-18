#pragma once


typedef struct _USBD_CDC_Itf
{
    int8_t(*Init)          (void);
    int8_t(*DeInit)        (void);
    int8_t(*Control)       (uint8_t, uint8_t *, uint16_t);
    int8_t(*Receive)       (uint8_t *, uint32_t *);

}USBD_CDC_ItfTypeDef;

typedef struct
{
    __IO uint32_t TxState;
}
USBD_CDC_HandleTypeDef;

extern USBD_ClassTypeDef  USBD_CDC;

uint8_t  USBD_CDC_RegisterInterface(USBD_HandleTypeDef *pdev, USBD_CDC_ItfTypeDef *fops);
uint8_t  USBD_CDC_SetTxBuffer(USBD_HandleTypeDef *pdev, uint8_t  *pbuff, uint16_t length);
uint8_t  USBD_CDC_TransmitPacket(USBD_HandleTypeDef *pdev);
