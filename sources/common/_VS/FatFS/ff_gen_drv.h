#pragma once

#include "diskio.h"
#include "ff.h"
#include "stdint.h"


#define BYTE    char
#define DSTATUS char
#define DRESULT char

struct Diskio_drvTypeDef
{
     DSTATUS(*disk_initialize) (BYTE);                      /*!< Initialize Disk Drive  */
     DSTATUS(*disk_status)     (BYTE);                      /*!< Get Disk Status        */
     DRESULT(*disk_read)       (BYTE, BYTE*, DWORD, UINT);  /*!< Read Sector(s)         */
};

uint8_t FATFS_LinkDriver(const Diskio_drvTypeDef *drv, char *path);
