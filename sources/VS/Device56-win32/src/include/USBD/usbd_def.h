#pragma once


struct USBD_HandleTypeDef
{
    int i;
};


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