#include "defines.h"
#include "device.h"
#ifdef GUI
#include "Osci/Osci_win.h"
#include "Recorder/Recorder_win.h"
#endif
#include <stm32f4xx_hal.h>



void HAL_GPIO_WritePin(GPIO_TypeDef *, uint16_t, GPIO_PinState)
{

}


void  HAL_GPIO_Init(GPIO_TypeDef  *, GPIO_InitTypeDef *)
{

}

void  HAL_GPIO_DeInit(GPIO_TypeDef  *, uint32_t)
{

}

#ifdef GUI

GPIO_PinState HAL_GPIO_ReadPin(const GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin) //-V2506
{
    if (GPIOx == GPIOG && GPIO_Pin == GPIO_PIN_1)                              // Чтение готовности точки регистратора //-V2571
    {
        if(Device::InModeRecorder())
        {
            return RecorderHAL::ReadyPoint() ? GPIO_PIN_SET : GPIO_PIN_RESET;
        }
        else if(Device::InModeOsci())
        {
            return OsciHAL::ReadyPoint() ? GPIO_PIN_SET : GPIO_PIN_RESET;
        }
        else
        {
            // здесь ничего
        }
    }

    return GPIO_PIN_RESET;
}

#else

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *, uint16_t)
{
    return GPIO_PIN_RESET;
}

#endif



void HAL_GPIO_EXTI_IRQHandler(uint16_t)
{

}
