#include "defines.h"
#include "HAL.h"
#include <stm32f4xx_hal.h>



static CRC_HandleTypeDef handle = { CRC };


void HAL::CRC32_::Init()
{
    if (HAL_CRC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


uint HAL::CRC32_::Calculate(uint address, uint size)
{
    return HAL_CRC_Calculate(&handle, (uint *)address, size);
}
