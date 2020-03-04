#pragma once


typedef struct _USBH_HandleTypeDef USBH_HandleTypeDef;


class Sensor
{
public:
    static void Init();
    static void DeInit();
private:
    static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8 id);
};
