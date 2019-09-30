#include "defines.h"
#include "HAL.h"
#include "Tester/Tester.h"
#include <stm32f4xx_hal.h>


using namespace HAL::PIO;



static DAC_HandleTypeDef handle = { DAC };



void HAL::DAC2_::Init()
{
}


void HAL::DAC2_::SetValue(uint value)
{
    HAL_DAC_SetValue(&handle, DAC1_CHANNEL_2, DAC_ALIGN_8B_R, value);
}
