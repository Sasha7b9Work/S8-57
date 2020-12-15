#include "defines.h"
#include "Hardware/Battery.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>


/*
*   Таймер использовался для выключения прибора
*/

static TIM_HandleTypeDef handleTIM5 = { TIM5 }; //-V2571


void HAL_TIM5::Init()
{
/*
    HAL_NVIC_SetPriority(TIM5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);

    __HAL_RCC_TIM5_CLK_ENABLE();

    handleTIM5.Init.Prescaler = 20000;
    handleTIM5.Init.CounterMode = TIM_COUNTERMODE_UP;
    handleTIM5.Init.Period = 20000;
    handleTIM5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;

    HAL_TIM_Base_Init(&handleTIM5);

    HAL_TIM_Base_Start_IT(&handleTIM5);
*/
}

/*
void HAL_TIM5::ElapsedCallback()
{
    if(!Battery::IsBusy())
    {
        if (HAL_ADC1::ValueBattery() < 500)
        {
            HAL_PIO::Reset(PIN_POWER);
        }
    }
    else
    {
        if (Battery::GetVoltageAverage() < 5.0F)
        {
            NVIC_SystemReset();
        }
    }
}
*/
