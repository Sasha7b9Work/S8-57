#pragma once


typedef struct _USBD_CDC_Itf
{
    int8_t(*Init)          (void);
    int8_t(*DeInit)        (void);
    int8_t(*Control)       (uint8_t, uint8_t *, uint16_t);
    int8_t(*Receive)       (uint8_t *, uint32_t *);

}USBD_CDC_ItfTypeDef;
