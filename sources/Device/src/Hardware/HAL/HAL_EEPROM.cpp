#include "defines.h"
#include "HAL.h"
#include "Hardware/Beeper.h"
#include <stm32f4xx_hal.h>



#define CLEAR_FLASH_FLAGS                                                                   \
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP     |  /* end of operation flag              */   \
                            FLASH_FLAG_OPERR  |  /* operation error flag               */   \
                            FLASH_FLAG_WRPERR |  /* write protected error flag         */   \
                            FLASH_FLAG_PGAERR |  /* programming alignment error flag   */   \
                            FLASH_FLAG_PGPERR |  /* programming parallelism error flag */   \
                            FLASH_FLAG_PGSERR);  /* programming sequence error flag    */


#define  _16K (16 * 1024)
#define  _64K (64 * 1024)
#define _128K (128 * 1024)


static uint GetSector(uint strtAddress);


const uint HAL_FLASH::Sector::address[HAL_FLASH::Sector::NUM_SECTORS] =
{
    0x08000000U,    // 00_BOOT_1
    0x08004000U,    // 01_BOOT_2
    0x08008000U,    // 02
    0x0800C000U,    // 03
    0x08010000U,    // 04
    0x08020000U,    // 05_FIRM_1
    0x08040000U,    // 06_FIRM_2
    0x08060000U,    // 07_FIRM_3
    0x08080000U,    // 08
    0x080A0000U,    // 09
    0x080C0000U,    // 10_SETTINGS_1
    0x080E0000U,    // 11_SETTINGS_2
    0x08100000U,    // 12
    0x08104000U,    // 13
    0x08108000U,    // 14
    0x0810C000U,    // 15
    0x08110000U,    // 16
    0x08120000U,    // 17_RECORDER_1
    0x08140000U,    // 18_RECORDER_2
    0x08160000U,    // 19_DATA_1
    0x08180000U,    // 20_DATA_2
    0x081A0000U,    // 21_DATA_3
    0x081C0000U,    // 22_DATA_4
    0x081E0000U     // 23_DATA_5
};


const uint HAL_FLASH::Sector::size[HAL_FLASH::Sector::NUM_SECTORS] =
{
    _16K,   // 00_BOOT_1
    _16K,   // 01_BOOT_2
    _16K,   // 02
    _16K,   // 03
    _64K,   // 04
    _128K,  // 05_FIRM_1
    _128K,  // 06_FIRM_2
    _128K,  // 07_FIRM_3
    _128K,  // 08
    _128K,  // 09
    _128K,  // 10_SETTINGS_1
    _128K,  // 11_SETTINGS_2
    _16K,   // 12
    _16K,   // 13
    _16K,   // 14
    _16K,   // 15
    _64K,   // 16
    _128K,  // 17_RECORDER_1
    _128K,  // 18_RECORDER_2
    _128K,  // 19_DATA_1
    _128K,  // 20_DATA_2
    _128K,  // 21_DATA_3
    _128K,  // 22_DATA_4
    _128K   // 23_DATA_5
};



void HAL_FLASH::EraseSector(uint startAddress)
{
    CLEAR_FLASH_FLAGS;

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

    LOG_ERROR("Неправильный адрес");

    return 24;
}


void HAL_FLASH::WriteBytes(uint address, const uint8 *data, int size)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    for (int i = 0; i < size; i++)
    {
        if (HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, data[i]) != HAL_OK)
        {
            ERROR_HANDLER();
        }
        ++address;
    }

    HAL_FLASH_Lock();
}


void HAL_FLASH::WriteBufferBytes(uint address, const void *buffer, int size)
{
    Beeper::WaitForCompletion();

    CLEAR_FLASH_FLAGS

    HAL_FLASH_Unlock();

    for (int i = 0; i < size; i++)
    {
        uint64_t data = static_cast<uint8 *>(const_cast<void *>(buffer))[i];
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, data);
        address++;
    }

    HAL_FLASH_Lock();
}
