#include "defines.h"
#include <stm32f4xx_hal_adc.h>


static uint channel = 2;


extern "C"
{
    HAL_StatusTypeDef HAL_ADC_Init(ADC_HandleTypeDef* /*hadc*/)
    {
        return HAL_OK;
    }


    HAL_StatusTypeDef HAL_ADC_ConfigChannel(ADC_HandleTypeDef* /*hadc*/, ADC_ChannelConfTypeDef* config) //-V2009 //-V2558
    {
        channel = config->Channel;

        return HAL_OK;
    }


    uint32_t HAL_ADC_GetValue(ADC_HandleTypeDef * /*hadc*/) //-V2506
    {
        uint max = (1 << 12) - 1;

        if (channel == 2)
        {
            return max * 50 / 100;
        }

        return max;
    }


    HAL_StatusTypeDef HAL_ADC_Start(ADC_HandleTypeDef* /*hadc*/)
    {
        return HAL_OK;
    }

    
    HAL_StatusTypeDef HAL_ADC_PollForConversion(ADC_HandleTypeDef* /*hadc*/, uint32_t /*Timeout*/)
    {
        return HAL_OK;
    }

    
    HAL_StatusTypeDef HAL_ADC_Start_IT(ADC_HandleTypeDef* /*hadc*/)
    {
        return HAL_OK;
    }

    
    void HAL_ADC_IRQHandler(ADC_HandleTypeDef* /*hadc*/)
    {

    }
}
