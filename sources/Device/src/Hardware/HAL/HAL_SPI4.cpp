#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


void HAL_SPI4::Init()
{
    __HAL_RCC_SPI4_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO;
    // SPI4 GPIO Configuration    
    // PE11     ------> SPI4_NSS
    // PE12     ------> SPI4_SCK
    // PE13     ------> SPI4_MISO
    // PE14     ------> SPI4_MOSI 
    isGPIO.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
    isGPIO.Mode = GPIO_MODE_AF_PP;
    isGPIO.Pull = GPIO_NOPULL;
    isGPIO.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    isGPIO.Alternate = GPIO_AF5_SPI4;
    HAL_GPIO_Init(GPIOE, &isGPIO);
}
