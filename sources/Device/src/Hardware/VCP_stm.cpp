#include "defines.h"
#include "usbd_desc.h"
#include "usbd_cdc_interface.h"
#include "Hardware/VCP.h"
#include "usbd_desc.h"
#include "Utils/Math.h"
#include <cstring>
#include <cstdarg>
#include <usbd_core.h>
#include <cstdio>



USBD_HandleTypeDef hUSBD;
PCD_HandleTypeDef  hPCD;

void *VCP::handleUSBD = &hUSBD;
void *VCP::handlePCD = &hPCD;
bool VCP::cableUSBisConnected = false;
bool VCP::connectedToUSB = false;



static bool PrevSendingComplete();



void VCP::Init()
{
    USBD_Init(&hUSBD, &VCP_Desc, DEVICE_FS);
    USBD_RegisterClass(&hUSBD, &USBD_CDC);
    USBD_CDC_RegisterInterface(&hUSBD, &USBD_CDC_fops);
    USBD_Start(&hUSBD);
}


static bool PrevSendingComplete()
{
    USBD_CDC_HandleTypeDef *pCDC = static_cast<USBD_CDC_HandleTypeDef *>(hUSBD.pClassData); //-V2571
    return pCDC->TxState == 0;
}


void VCP::SendDataAsynch(const uint8 *buffer, int size)
{
#define SIZE_BUFFER 64
    static uint8 trBuf[SIZE_BUFFER];

    size = Math::Min(size, SIZE_BUFFER);
    while (!PrevSendingComplete())  {};
    std::memcpy(trBuf, buffer, static_cast<uint>(size));

    USBD_CDC_SetTxBuffer(&hUSBD, trBuf, static_cast<uint16>(size));
    USBD_CDC_TransmitPacket(&hUSBD);
}


#define SIZE_BUFFER_VCP 256     // \todo если поставить размер буфера 512, то на ТЕ207 глюки
static uint8 buffSend[SIZE_BUFFER_VCP];
static int sizeBuffer = 0;


void VCP::Flush()
{
    if (sizeBuffer)
    {
        volatile USBD_CDC_HandleTypeDef *pCDC = static_cast<USBD_CDC_HandleTypeDef *>(hUSBD.pClassData); //-V2571

        while (pCDC->TxState == 1) {}; //-V712

        USBD_CDC_SetTxBuffer(&hUSBD, buffSend, static_cast<uint16>(sizeBuffer));
        USBD_CDC_TransmitPacket(&hUSBD);

        while (pCDC->TxState == 1) {}; //-V654 //-V712
    }
    sizeBuffer = 0;
}


void VCP::SendDataSynch(const void *_buffer, int size)
{
    if (VCP::connectedToUSB)
    {
        char *buffer = static_cast<char *>(const_cast<void *>(_buffer)); //-V2567 //-V2571
        if (size == 0)
        {
            size = static_cast<int>(std::strlen(buffer)); //-V2513
        }

        volatile USBD_CDC_HandleTypeDef *pCDC = static_cast<USBD_CDC_HandleTypeDef *>(hUSBD.pClassData); //-V2571
    
        do 
        {
            if (sizeBuffer + size > SIZE_BUFFER_VCP)
            {
                int reqBytes = SIZE_BUFFER_VCP - sizeBuffer;
                LIMITATION(reqBytes, 0, static_cast<int>(size));

                while (pCDC->TxState == 1) {}; //-V712

                std::memcpy(buffSend + sizeBuffer, static_cast<void *>(buffer), static_cast<uint>(reqBytes)); //-V2563
                USBD_CDC_SetTxBuffer(&hUSBD, buffSend, SIZE_BUFFER_VCP);
                USBD_CDC_TransmitPacket(&hUSBD);
                size -= reqBytes;
                buffer += reqBytes; //-V2563
                sizeBuffer = 0;
            }
            else
            {
                std::memcpy(buffSend + sizeBuffer, static_cast<void *>(buffer), static_cast<uint>(size)); //-V2563
                sizeBuffer += size;
                size = 0;
            }
        } while (size);
    }
}


void VCP::SendStringAsynch(const char *data)
{
    SendDataAsynch(reinterpret_cast<uint8 *>(const_cast<char *>(data)), static_cast<int>(std::strlen(data))); //-V2513 //-V2567
}


void VCP::SendStringSynch(char *data)
{
    SendDataSynch(reinterpret_cast<uint8 *>(data), static_cast<int>(std::strlen(data))); //-V2513
}


void VCP::SendFormatStringAsynch(char *format, ...)
{
    if (VCP::connectedToUSB)
    {
        static char buffer[200];
        std::va_list args;
        va_start(args, format); //-V2528
        std::vsprintf(buffer, format, args);
        va_end(args);
        std::strcat(buffer, "\r\n"); //-V2513
        SendDataAsynch(reinterpret_cast<uint8 *>(buffer), static_cast<int>(std::strlen(buffer))); //-V2513
    }
}


void VCP::SendFormatStringSynch(char *format, ...)
{
    char buffer[200];
    std::va_list args;
    va_start(args, format); //-V2528
    std::vsprintf(buffer, format, args);
    va_end(args);
    std::strcat(buffer, "\r\n"); //-V2513
    SendDataSynch(reinterpret_cast<uint8 *>(buffer), static_cast<int>(std::strlen(buffer))); //-V2513
}


void VCP::SendByte(uint8 byte)
{
    SendDataSynch(&byte, 1);
}
