#include "defines.h"
#include "Recorder/Sensor.h"
#include <usbh_cdc.h>
#include <usbh_core.h>
#include <usbh_def.h>


static USBH_HandleTypeDef handle;


void Sensor::Init()
{
    USBH_Init(&handle, Sensor::USBH_UserProcess, 0);
    USBH_RegisterClass(&handle, USBH_CDC_CLASS);
    USBH_Start(&handle);
}


void Sensor::DeInit()
{

}


void Sensor::USBH_UserProcess(USBH_HandleTypeDef *, uint8)
{

}
