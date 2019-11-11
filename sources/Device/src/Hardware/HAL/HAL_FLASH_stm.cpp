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


const Sector HAL_ROM::sectors[Sector::Count] =
{
    { Sector::_00_BOOT_1,     0x08000000U, _16K },
    { Sector::_01_BOOT_2,     0x08004000U, _16K },
    { Sector::_02,            0x08008000U, _16K },
    { Sector::_03,            0x0800C000U, _16K },
    { Sector::_04,            0x08010000U, _64K },
    { Sector::_05_FIRM_1,     0x08020000U, _128K },
    { Sector::_06_FIRM_2,     0x08040000U, _128K },
    { Sector::_07_FIRM_3,     0x08060000U, _128K },
    { Sector::_08,            0x08080000U, _128K },
    { Sector::_09,            0x080A0000U, _128K },
    { Sector::_10_SETTINGS_1, 0x080C0000U, _128K },
    { Sector::_11_SETTINGS_2, 0x080E0000U, _128K },
    { Sector::_12,            0x08100000U, _16K },
    { Sector::_13,            0x08104000U, _16K },
    { Sector::_14,            0x08108000U, _16K },
    { Sector::_15,            0x0810C000U, _16K },
    { Sector::_16,            0x08110000U, _64K },
    { Sector::_17_RECORDER_1, 0x08120000U, _128K },
    { Sector::_18_RECORDER_2, 0x08140000U, _128K },
    { Sector::_19_DATA_1,     0x08160000U, _128K },
    { Sector::_20_DATA_2,     0x08180000U, _128K },
    { Sector::_21_DATA_3,     0x081A0000U, _128K },
    { Sector::_22_DATA_4,     0x081C0000U, _128K },
    { Sector::_23_DATA_5,     0x081E0000U, _128K }
};


void Sector::Erase() const
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef isFLASH =
    {
        FLASH_TYPEERASE_SECTORS,
        0,
        static_cast<uint>(number),
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


void HAL_ROM::WriteBytes(uint address, const uint8 *data, uint size)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    for (uint i = 0; i < size; i++)
    {
        if (HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, data[i]) != HAL_OK)
        {
            ERROR_HANDLER();
        }
        ++address;
    }

    HAL_FLASH_Lock();
}


void HAL_ROM::WriteBufferBytes(uint address, const void *buffer, uint size)
{
    Beeper::WaitForCompletion();

    CLEAR_FLASH_FLAGS

    HAL_FLASH_Unlock();

    for (uint i = 0; i < size; i++)
    {
        uint64_t data = static_cast<uint8 *>(const_cast<void *>(buffer))[i];
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, data);
        address++;
    }

    HAL_FLASH_Lock();
}
