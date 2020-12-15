#include "defines.h"
#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>
#include <usbh_core.h>
#include "CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"



void CPU::Init()
{
    STM437::Init();

    EnablePeriphery();

    InitHardware();

    // Таймер для мс
    HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);

    HAL_PIO::Init(PIN_POWER, HMode::Output_PP, HPull::Up);
    HAL_PIO::Set(PIN_POWER);
}


void CPU::DeInit()
{   
    HAL_DeInit();
}


void CPU::EnablePeriphery()
{
    __HAL_RCC_GPIOA_CLK_ENABLE(); //-V2571
    __HAL_RCC_GPIOB_CLK_ENABLE(); //-V2571
    __HAL_RCC_GPIOC_CLK_ENABLE(); //-V2571
    __HAL_RCC_GPIOD_CLK_ENABLE(); //-V2571
    __HAL_RCC_GPIOE_CLK_ENABLE(); //-V2571
    __HAL_RCC_GPIOF_CLK_ENABLE(); //-V2571
    __HAL_RCC_GPIOG_CLK_ENABLE(); //-V2571
    __HAL_RCC_SPI1_CLK_ENABLE(); //-V2571
    __HAL_RCC_FMC_CLK_ENABLE(); //-V2571
    __HAL_RCC_USB_OTG_FS_CLK_ENABLE(); //-V2571
    __HAL_RCC_DMA1_CLK_ENABLE();        // Для DAC1 (бикалка) //-V2571
    __HAL_RCC_TIM7_CLK_ENABLE();        // Для DAC1 (бикалка) //-V2571
    __HAL_RCC_DAC_CLK_ENABLE();         // Бикалка //-V2571
    __PWR_CLK_ENABLE(); //-V2571
    __SYSCFG_CLK_ENABLE(); //-V2571
}


void CPU::InitHardware()
{
   Timer::Init();

   HAL::Init();
}
