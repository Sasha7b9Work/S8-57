// 2023/11/22 13:45:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


typedef struct _USBH_HandleTypeDef USBH_HandleTypeDef;


class Sensor
{
public:
    static void Init();
    static void DeInit();
    static void Update();
    static bool IsActive();
};
