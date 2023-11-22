// 2023/11/22 13:39:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <usbd_def.h>


namespace VCP
{
    static const int DEVICE_FS = 0;

    // Инициализация
    void Init();

    void SendDataAsynch(const uint8 *data, int size);

    void SendDataSynch(const void *data, int size = 0);

    // Передаётся строка без завершающего нуля
    void SendStringAsynch(const char *data);

    // Передаётся строка без завершающего нуля
    void SendStringSynch(char *data);

    // Эта строка передаётся с завершающими символами \\r\\n
    void SendFormatStringAsynch(char *format, ...);

    // Эта строка передаётся с завершающими символами \\r\\n
    void SendFormatStringSynch(char *format, ...);

    void SendByte(uint8 data);

    void Flush();

    extern bool connectedToUSB;

    extern bool cableUSBisConnected;

    extern void *handleUSBD;

    extern void *handlePCD;
};
