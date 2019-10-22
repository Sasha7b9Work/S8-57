#include "defines.h"
#include "HAL.h"
#include <stm32f4xx_hal.h>


uint HAL_NVIC::irqEXTI1 = EXTI1_IRQn;
uint HAL_NVIC::irqEXTI9_5 = EXTI9_5_IRQn;


void HAL_NVIC::EnableIRQ(uint irq)
{
    HAL_NVIC_EnableIRQ(static_cast<IRQn_Type>(irq));
}


void HAL_NVIC::DisableIRQ(uint irq)
{
    HAL_NVIC_DisableIRQ(static_cast<IRQn_Type>(irq));
}


void HAL_NVIC::SetPriorityIRQ(uint irq, uint main, uint sub)
{
    HAL_NVIC_SetPriority(static_cast<IRQn_Type>(irq), main, sub);
}
