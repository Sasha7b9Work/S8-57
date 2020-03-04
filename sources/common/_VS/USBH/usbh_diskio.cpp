#include "defines.h"
#include "usbh_diskio.h"
#include "ff_gen_drv.h"


DSTATUS USBH_initialize(BYTE);
DSTATUS USBH_status(BYTE);
DRESULT USBH_read(BYTE, BYTE*, DWORD, UINT);


const Diskio_drvTypeDef  USBH_Driver =
{
    USBH_initialize,
    USBH_status,
    USBH_read
};
