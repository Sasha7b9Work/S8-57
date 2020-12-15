#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>


void HAL_SPI4::Init()
{
    __HAL_RCC_SPI4_CLK_ENABLE(); //-V2571

    HAL_PIO::Init(PIN_SPI4_CS, HMode::AF_PP, HPull::No, HSpeed::VeryHigh, HAlternate::AF5_SPI4);
    HAL_PIO::Init(PIN_SPI4_SCK, HMode::AF_PP, HPull::No, HSpeed::VeryHigh, HAlternate::AF5_SPI4);
    HAL_PIO::Init(PIN_SPI4_MOSI, HMode::AF_PP, HPull::No, HSpeed::VeryHigh, HAlternate::AF5_SPI4);
    HAL_PIO::Init(PIN_SPI4_MISO, HMode::AF_PP, HPull::No, HSpeed::VeryHigh, HAlternate::AF5_SPI4);
}
