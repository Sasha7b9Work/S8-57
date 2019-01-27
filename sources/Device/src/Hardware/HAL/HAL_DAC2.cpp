#include "defines.h"
#include "HAL.h"
#include "Tester/Tester.h"
#include <stm32f4xx_hal.h>


using namespace HAL::PIO;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static DAC_HandleTypeDef handle = { DAC };


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HAL::DAC2_::Init()
{
    RCC->APB1ENR |= RCC_APB1ENR_DACEN;      // Включаем ЦАП

    HAL::PIO::Init(Port::_A, Pin::_5, Mode::Analog, Pull::No);    // Настраиваем выходной порт

    if (HAL_DAC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    //                         TEST_ON               PNP               U
    uint pins = (uint)(Tester::Pin_TEST_ON | Tester::Pin_PNP | Tester::Pin_U);
    HAL::PIO::Init(Port_TEST_ON, pins, Mode::Output_PP, Pull::Down);

    //                               I
    HAL::PIO::Init(Port_I, Tester::Pin_I, Mode::Output_PP, Pull::Down);

    //              TEST_STR - EXTI9
    HAL::PIO::Init(Port_TEST_STR, Tester::Pin_TEST_STR, Mode::RisingIT, Pull::No);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);

    HAL::PIO::Set(Port_TEST_ON, Tester::Pin_TEST_ON);         // Отключаем тестер-компонет

    // Инициализируем ЦАП
    GPIO_InitTypeDef _gpio =
    {
        GPIO_PIN_5,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL
    };
    HAL_GPIO_Init(GPIOA, &_gpio);

    if (HAL_DAC_Init(&handle) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    DAC_ChannelConfTypeDef configDAC;
    configDAC.DAC_Trigger = DAC_TRIGGER_NONE;
    configDAC.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;

    if (HAL_DAC_ConfigChannel(&handle, &configDAC, DAC1_CHANNEL_2) != HAL_OK)
    {
        ERROR_HANDLER();
    }
    HAL_DAC_SetValue(&handle, DAC1_CHANNEL_2, DAC_ALIGN_8B_R, 0);
    HAL_DAC_Start(&handle, DAC1_CHANNEL_2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL::DAC2_::SetValue(uint value)
{
    HAL_DAC_SetValue(&handle, DAC1_CHANNEL_2, DAC_ALIGN_8B_R, value);
}
