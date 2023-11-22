#include "defines.h"
#include "HAL.h"
#include <stm32f4xx_hal.h>



#define CLEAR_FLASH_FLAGS                                                                   \
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP     |  /* end of operation flag              */   \
                            FLASH_FLAG_OPERR  |  /* operation error flag               */   \
                            FLASH_FLAG_WRPERR |  /* write protected error flag         */   \
                            FLASH_FLAG_PGAERR |  /* programming alignment error flag   */   \
                            FLASH_FLAG_PGPERR |  /* programming parallelism error flag */   \
                            FLASH_FLAG_PGSERR);  /* programming sequence error flag    */



static uint GetSector(uint strtAddress);



void HAL_EEPROM::EraseSector(uint startAddress)
{
    CLEAR_FLASH_FLAGS; //-V2571

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef isFLASH =
    {
        FLASH_TYPEERASE_SECTORS,
        0,
        GetSector(startAddress),
        1,
        FLASH_VOLTAGE_RANGE_3
    };

    uint error = 0;

    if (HAL_FLASHEx_Erase(&isFLASH, &error) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_FLASH_Lock();
}


static uint GetSector(uint startAddress)
{
    static const uint addresses[24] =
    {
        0x08000000,     // FLASH_SECTOR_0
        0x08004000,     // FLASH_SECTOR_1
        0x08008000,     // FLASH_SECTOR_2
        0x0800C000,     // FLASH_SECTOR_3
        0x08010000,     // FLASH_SECTOR_4
        0x08020000,     // FLASH_SECTOR_5
        0x08040000,     // FLASH_SECTOR_6
        0x08060000,     // FLASH_SECTOR_7
        0x08080000,     // FLASH_SECTOR_8
        0x080A0000,     // FLASH_SECTOR_9
        0x080C0000,     // FLASH_SECTOR_10
        0x080E0000,     // FALSH_SECTOR_11

        0x08100000,     // FLASH_SECTOR_12
        0x08104000,     // FLASH_SECTOR_13
        0x08108000,     // FLASH_SECTOR_14
        0x0810C000,     // FLASH_SECTOR_15
        0x08110000,     // FLASH_SECTOR_16
        0x08120000,     // FLASH_SECTOR_17
        0x08140000,     // FLASH_SECTOR_18
        0x08160000,     // FLASH_SECTOR_19
        0x08180000,     // FLASH_SECTOR_20
        0x081A0000,     // FLASH_SECTOR_21
        0x081C0000,     // FLASH_SECTOR_22
        0x081E0000      // FLASH_SECTOR_23
    };

    for (uint i = 0; i < 24; i++)
    {
        if (addresses[i] == startAddress)
        {
            return i;
        }
    }

    return 24;
}


void HAL_EEPROM::WriteBytes(uint address, const uint8 *data, int size)
{
    CLEAR_FLASH_FLAGS; //-V2571

    HAL_FLASH_Unlock();

    for (int i = 0; i < size; i++)
    {
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, data[i]);
        ++address;
    }

    HAL_FLASH_Lock();
}
