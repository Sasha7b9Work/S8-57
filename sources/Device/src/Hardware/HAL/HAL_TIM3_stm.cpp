#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


static TIM_HandleTypeDef handlerTIM3 = { TIM3 }; //-V2571


void HAL_TIM3::Init(uint prescaler, uint period)
{
    __HAL_RCC_TIM3_CLK_ENABLE(); //-V2571

    handlerTIM3.Init.Prescaler = prescaler;
    handlerTIM3.Init.CounterMode = TIM_COUNTERMODE_UP;
    handlerTIM3.Init.Period = period;
    handlerTIM3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_DeInit(&handlerTIM3);
}


void HAL_TIM3::Start()
{
    HAL_TIM_Base_Start(&handlerTIM3);
}


void HAL_TIM3::Stop()
{
    HAL_TIM_Base_Stop(&handlerTIM3);
}


void HAL_TIM3::DeInit()
{
    __HAL_RCC_TIM3_CLK_DISABLE(); //-V2571
}


void HAL_TIM3::EnableIRQ(uint mainPriority, uint subPriority)
{
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
    HAL_NVIC_SetPriority(TIM3_IRQn, mainPriority, subPriority);
}


void HAL_TIM3::DisableIRQ()
{
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
}


void HAL_TIM3::StartIT(uint period)
{
    handlerTIM3.Init.Period = period;
    HAL_TIM_Base_Init(&handlerTIM3);
    HAL_TIM_Base_Start_IT(&handlerTIM3);
}


void HAL_TIM3::StopIT()
{
    HAL_TIM_Base_Stop_IT(&handlerTIM3);
}
