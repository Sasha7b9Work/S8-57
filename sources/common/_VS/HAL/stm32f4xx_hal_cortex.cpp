#include "stdafx.h"
#include "stm32f4xx_hal_cortex.h"


extern "C"
{

    
    void HAL_NVIC_DisableIRQ(IRQn_Type)
    {
    
    }

    
    uint32_t HAL_SYSTICK_Config(uint32_t)
    {
        return 0;
    }

    
    void HAL_NVIC_EnableIRQ(IRQn_Type)
    {

    }

    
    void HAL_NVIC_SetPriority(IRQn_Type, uint32_t, uint32_t)
    {

    }

    void HAL_NVIC_SystemReset()
    {

    }

    void HAL_NVIC_SetPriorityGrouping(uint32_t)
    {

    }

    void HAL_SYSTICK_IRQHandler()
    {

    }

    void HAL_SYSTICK_CLKSourceConfig(uint32_t)
    {

    }
}
