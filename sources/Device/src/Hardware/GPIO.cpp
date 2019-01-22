#include <stm32f4xx_hal.h>
#include "defines.h"
#include "GPIO.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GPIO gpio;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GPIO::SetInputPullDown(GPIO_TypeDef *pio, uint numPin)
{
    pio->MODER &= ~(3U << (numPin * 2));

    pio->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (numPin * 2U)); //-V684
    pio->PUPDR |= GPIO_PULLDOWN << (numPin * 2U);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GPIO::SetOutputPP_PullDown(GPIO_TypeDef *pio, uint numPin)
{
    pio->MODER &= ~(GPIO_MODER_MODER0 << (numPin * 2));
    pio->MODER |= (GPIO_MODE_OUTPUT_PP << (numPin * 2));

    pio->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (numPin * 2U)); //-V684
    pio->PUPDR |= GPIO_PULLDOWN << (numPin * 2U);
}
