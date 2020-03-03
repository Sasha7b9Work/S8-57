#include "defines.h"
#include "ff_gen_drv.h"


const Diskio_drvTypeDef USBH_Driver{ 0, 0, 0 };



uint8_t FATFS_LinkDriver(const Diskio_drvTypeDef * /*drv*/, char * /*path*/)
{
    return 0;
}


uint8_t FATFS_UnLinkDriver(char * /*path*/)
{
    return 0;
}
