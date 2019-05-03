#include "defines.h"
#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>
#include <usbh_core.h>
#include "CPU.h"
#include "Hardware/Timer.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::Init()
{
    STM437::Init();

    EnablePeriphery();

    InitHardware();

    // Таймер для мс
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::DeInit()
{   
    HAL_DeInit();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::EnablePeriphery()
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_FMC_CLK_ENABLE();
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    __HAL_RCC_DMA1_CLK_ENABLE();        // Для DAC1 (бикалка)
    __HAL_RCC_TIM7_CLK_ENABLE();        // Для DAC1 (бикалка)
    __HAL_RCC_DAC_CLK_ENABLE();         // Бикалка
    __PWR_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::InitHardware()
{
   Timer::Init();
}
