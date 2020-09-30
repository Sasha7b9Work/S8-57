#include "defines.h"
#include "SCPI/SCPI.h"
#include "Log.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include <usbd_cdc.h>


static USBD_CDC_LineCodingTypeDef LineCoding =
{
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* nb. of bits 8*/
};

#define APP_RX_DATA_SIZE  64
static uint8_t UserRxBuffer[APP_RX_DATA_SIZE];/* Received Data over USB are stored in this buffer */


static int8_t CDC_Itf_Init     ();
static int8_t CDC_Itf_DeInit   ();
static int8_t CDC_Itf_Control  (uint8 cmd, uint8* pbuf, uint16 length);
static int8_t CDC_Itf_Receive  (uint8* pbuf, uint *Len);


USBD_CDC_ItfTypeDef USBD_CDC_fops = 
{
    CDC_Itf_Init,
    CDC_Itf_DeInit,
    CDC_Itf_Control,
    CDC_Itf_Receive
};


static void SetAttributeConnected()
{
    VCP::cableUSBisConnected = true;
    VCP::connectedToUSB = false;
}


static int8_t CDC_Itf_Init()
{
    USBD_CDC_SetRxBuffer(reinterpret_cast<USBD_HandleTypeDef *>(VCP::handleUSBD), UserRxBuffer);
    Timer::SetAndStartOnce(TypeTimer::USB, SetAttributeConnected, 100);   /** \todo Задержка введена для того, чтобы не было ложных срабатываний в 
                                                                 usbd_conf.c:HAL_PCD_SetupStageCallback при определении подключения хоста */
    return (USBD_OK);
}


static int8_t CDC_Itf_DeInit()
{
    VCP::cableUSBisConnected = false;
    VCP::connectedToUSB = false;

    return (USBD_OK);
}


static int8_t CDC_Itf_Control (uint8 cmd, uint8* pbuf, uint16)
{ 
    switch (cmd)
    {
    case CDC_SEND_ENCAPSULATED_COMMAND:
        break;
        
    case CDC_GET_ENCAPSULATED_RESPONSE:
        break;

    case CDC_SET_COMM_FEATURE:
        break;

    case CDC_GET_COMM_FEATURE:
        break;

    case CDC_CLEAR_COMM_FEATURE:
        break;

    case CDC_SET_LINE_CODING:
        LineCoding.bitrate    = static_cast<uint32_t>(pbuf[0] | (pbuf[1] << 8) | (pbuf[2] << 16) | (pbuf[3] << 24));
        LineCoding.format     = pbuf[4];
        LineCoding.paritytype = pbuf[5];
        LineCoding.datatype   = pbuf[6];
        break;

    case CDC_GET_LINE_CODING:
        pbuf[0] = static_cast<uint8_t>(LineCoding.bitrate);
        pbuf[1] = static_cast<uint8_t>(LineCoding.bitrate >> 8);
        pbuf[2] = static_cast<uint8_t>(LineCoding.bitrate >> 16);
        pbuf[3] = static_cast<uint8_t>(LineCoding.bitrate >> 24);
        pbuf[4] = LineCoding.format;
        pbuf[5] = LineCoding.paritytype;
        pbuf[6] = LineCoding.datatype;
        break;

    case CDC_SET_CONTROL_LINE_STATE:
        break;

    case CDC_SEND_BREAK:
        break;
    }
  
    return (USBD_OK);
}


static int8_t CDC_Itf_Receive(uint8 *buffer, uint *length) //-V2009 //-V2558
{
    SCPI::AppendNewData(reinterpret_cast<const char *>(buffer), *reinterpret_cast<int *>(length));

    USBD_CDC_ReceivePacket(reinterpret_cast<USBD_HandleTypeDef *>(VCP::handleUSBD));

    return (USBD_OK);
}
