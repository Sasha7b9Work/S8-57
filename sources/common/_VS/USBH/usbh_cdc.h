#pragma once
#include <usbh_def.h>


#define USB_CDC_CLASS                                           0x02


extern USBH_ClassTypeDef  CDC_Class;
#define USBH_CDC_CLASS    &CDC_Class
