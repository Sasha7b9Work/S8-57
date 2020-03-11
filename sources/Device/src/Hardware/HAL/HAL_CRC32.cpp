#include "defines.h"
#include "HAL.h"
#include <stm32f4xx_hal.h>


static CRC_HandleTypeDef handle = { CRC };


void HAL_CRC32::Init()
{
    if (HAL_CRC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


uint HAL_CRC32::Calculate8bit(uint8 *buffer, uint size)
{
    if((size % 4) != 0)
    {
        size -= (size % 4);
    }

    return HAL_CRC_Calculate(&handle, reinterpret_cast<uint *>(buffer), size / 4);
}
