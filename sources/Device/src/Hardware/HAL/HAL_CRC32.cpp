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


uint HAL_CRC32::Calculate(uint address, uint size)
{
    return HAL_CRC_Calculate(&handle, (uint *)address, size);
}
