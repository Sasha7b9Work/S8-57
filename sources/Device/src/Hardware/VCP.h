#pragma once
#include <usbd_def.h>


class VCP
{
public:
    static const int DEVICE_FS = 0;

    /// Инициализация
    static void Init();

    static void SendDataAsynch(const uint8 *data, uint size);

    static void SendDataSynch(const void *data, uint size = 0);
    /// Передаётся строка без завершающего нуля
    static void SendStringAsynch(const char *data);
    /// Передаётся строка без завершающего нуля
    static void SendStringSynch(char *data);
    /// Эта строка передаётся с завершающими символами \\r\\n
    static void SendFormatStringAsynch(char *format, ...);
    /// Эта строка передаётся с завершающими символами \\r\\n
    static void SendFormatStringSynch(char *format, ...);

    static void SendByte(uint8 data);

    static void Flush();

    static void *HandleUSBD();

    static void *HandlePCD();

    static bool connectedToUSB;

    static bool cableUSBisConnected;
};
