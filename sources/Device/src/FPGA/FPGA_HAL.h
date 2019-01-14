#pragma once
#include <stm32f4xx_hal.h>


namespace FPGA
{
    namespace HAL
    {
        void Init();

        void LoadRegUPR();

        ADC_HandleTypeDef *HandleADC();
    }
}
