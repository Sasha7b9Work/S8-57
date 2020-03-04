#pragma once


typedef struct _USBH_HandleTypeDef USBH_HandleTypeDef;


class Sensor
{
public:
    static void Init();
    static void DeInit();
    static void Update();
};
