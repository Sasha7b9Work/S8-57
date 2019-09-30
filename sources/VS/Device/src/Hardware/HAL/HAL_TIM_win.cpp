#include "defines.h"
#include "HAL.h"
#include <stm32f4xx_hal.h>




void HAL::TIM2_::Init(uint, uint)
{
}


void HAL::TIM2_::DeInit()
{
}


void HAL::TIM2_::Start()
{
}


void HAL::TIM2_::Stop()
{
}


void HAL::TIM2_::StartMultiMeasurement()
{
}


uint HAL::TIM2_::TimeUS()
{
    return HAL_GetTick() * 1000;
}


uint HAL::TIM2_::TimeTicks()
{
    return HAL_GetTick() * 1000 * 1000;
}



void HAL::TIM3_::Init(uint, uint)
{
}


void HAL::TIM3_::DeInit()
{
}


void HAL::TIM3_::EnableIRQ(uint, uint)
{
}


void HAL::TIM3_::DisableIRQ()
{
}


void HAL::TIM3_::StartIT(uint)
{
}


void HAL::TIM3_::StopIT()
{
}
