// 2024/04/16 15:32:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


//  WWDG clock counter = (PCLK1 (80MHz)/4096)/8) = 1525 Hz (~656 us)
//  WWDG Window value = 80 means that the WWDG counter should be refreshed only
//  when the counter is below 80 (and greater than 64/0x40) otherwise a reset will
//  be generated.
//  WWDG Counter value = 127, WWDG timeout = ~656 us * 64 = 42 ms


namespace HAL_WWDG
{
    static WWDG_HandleTypeDef handle;
}


void HAL_WWDG::Init()
{
    __HAL_RCC_WWDG_CLK_ENABLE();

    handle.Instance = WWDG;
    handle.Init.Prescaler = WWDG_PRESCALER_8;
    handle.Init.Window = 80;
    handle.Init.Counter = 127;

    HAL_WWDG_Init(&handle);
}


void HAL_WWDG::Update()
{
    HAL_WWDG_Refresh(&handle);
}
