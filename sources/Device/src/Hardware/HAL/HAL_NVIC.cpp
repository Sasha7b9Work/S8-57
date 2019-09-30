#include "defines.h"
#include "HAL.h"
#include <stm32f4xx_hal.h>



uint HAL::NVIC_::irqEXTI1 = EXTI1_IRQn;
uint HAL::NVIC_::irqEXTI9_5 = EXTI9_5_IRQn;



void HAL::NVIC_::EnableIRQ(uint irq)
{
    HAL_NVIC_EnableIRQ((IRQn_Type)irq);
}


void HAL::NVIC_::DisableIRQ(uint irq)
{
    HAL_NVIC_DisableIRQ((IRQn_Type)irq);
}


void HAL::NVIC_::SetPriorityIRQ(uint irq, uint main, uint sub)
{
    HAL_NVIC_SetPriority((IRQn_Type)irq, main, sub);
}
