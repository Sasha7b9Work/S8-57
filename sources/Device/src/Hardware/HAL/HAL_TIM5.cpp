#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


TIM_HandleTypeDef  inputCaptureHandleTIM;


void HAL_TIM5::Init()
{
    __HAL_RCC_TIM5_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);

    HAL_NVIC_EnableIRQ(TIM5_IRQn);
}


INTERRUPT_BEGIN

void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&inputCaptureHandleTIM);
}

INTERRUPT_END
