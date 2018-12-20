#include "stdafx.h"
#include "usbh_diskio.h"


DSTATUS USBH_initialize(BYTE);
DSTATUS USBH_status(BYTE);
DRESULT USBH_read(BYTE, BYTE*, DWORD, UINT);


const Diskio_drvTypeDef  USBH_Driver =
{
    USBH_initialize,
    USBH_status,
    USBH_read
};

DRESULT USBH_read(BYTE /*lun*/, BYTE * /*buff*/, DWORD /*sector*/, UINT /*count*/)
{
    return 0;
}

DSTATUS USBH_status(BYTE /*lun*/)
{
    return 0;
}

DSTATUS USBH_initialize(BYTE)
{
    return 0;
}
