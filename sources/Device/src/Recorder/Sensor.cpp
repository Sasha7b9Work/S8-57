#include "defines.h"
#include "log.h"
#include "Recorder/Sensor.h"
#include <usbh_cdc.h>
#include <usbh_core.h>
#include <usbh_def.h>


static USBH_HandleTypeDef handle;

static int requestForReceive = 0;

#define RX_BUFF_SIZE   0x400
static uint8 bufferRX[RX_BUFF_SIZE];



static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8 id);


void Sensor::Init()
{
    LOG_FUNC_ENTER();

    USBH_StatusTypeDef res = USBH_Init(&handle, USBH_UserProcess, 0);
    LOG_WRITE("1 = %d", res);

    res = USBH_RegisterClass(&handle, USBH_CDC_CLASS);
    LOG_WRITE("2 = %d", res);

    res = USBH_Start(&handle);
    LOG_WRITE("3 = %d", res);
}


void Sensor::DeInit()
{
    USBH_Stop(&handle);
    USBH_DeInit(&handle);
}


void Sensor::Update()
{
    USBH_Process(&handle);
}


void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    LOG_WRITE("USBH_UserProcess(%d)", id);

    switch(id)
    {
    case HOST_USER_SELECT_CONFIGURATION:
        break;

    case HOST_USER_DISCONNECTION:
        USBH_CDC_Stop(&handle);
        requestForReceive = 0;
        break;

    case HOST_USER_CLASS_ACTIVE:
        if(requestForReceive == 0)
        {
            std::memset(bufferRX, 0, RX_BUFF_SIZE);
            USBH_CDC_Receive(&handle, bufferRX, RX_BUFF_SIZE);
            requestForReceive = 1;
        }
        break;

    case HOST_USER_CONNECTION:
        break;

    default:
        break;
    }
}


void USBH_CDC_ReceiveCallback(USBH_HandleTypeDef *)
{
    uint16 size = USBH_CDC_GetLastReceivedDataSize(&handle);

    bufferRX[size] = 0;

    LOG_WRITE("%s", bufferRX);

    USBH_CDC_Receive(&handle, bufferRX, RX_BUFF_SIZE);
}
