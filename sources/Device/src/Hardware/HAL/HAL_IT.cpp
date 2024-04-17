// 2023/11/22 13:25:30 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Tester/Tester.h"
#include "Utils/Debug.h"
#include <stm32f4xx_hal.h>


INTERRUPT_BEGIN


void HardFault_Handler()
{
    __IO const char *file0 = Debug::file[0];
    __IO const char *file1 = Debug::file[1];
    __IO int line0 = Debug::line[0];
    __IO int line1 = Debug::line[1];
    __IO int line2 = Debug::line[2];
    
    while (1) //-V776
    {
        file0 = file0; //-V570
        file1 = file1; //-V570
        line0 = line0; //-V570
        line1 = line1; //-V570
        line2 = line2; //-V570

        NVIC_SystemReset();
    }
}


void SysTick_Handler(void)
{
    HAL_IncTick();

    if (Timer::watchdowg++ > 10000)
    {
        NVIC_SystemReset();
    }
}


void NMI_Handler(void)
{
}


void MemManage_Handler()
{
    while (1)
    {
        NVIC_SystemReset();
    }
}


void BusFault_Handler()
{
    while (1)
    {
        NVIC_SystemReset();
    }
}


void UsageFault_Handler()
{
    while (1)
    {
        NVIC_SystemReset();
    }
}


void SVC_Handler(void)
{
}


void DebugMon_Handler(void)
{
}


void PendSV_Handler(void)
{
}


void HAL_GPIO_EXTI_Callback(uint16 pin)
{
    if(pin == HPin::_9)      // Прерывание от тестер-компонента
    {
        Tester::ProcessStep();
    }
}


void TIM3_IRQHandler()
{
    if ((TIM3->SR & TIM_SR_UIF) == TIM_SR_UIF) //-V2571
    {
        if ((TIM3->DIER & TIM_DIER_UIE) == TIM_DIER_UIE) //-V2571
        {
            TIM3->SR = ~TIM_DIER_UIE; //-V2571
            Timer::ElapsedCallback();
        }
    }
}


void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);       // Тестер компонентов
}


// VCP
void OTG_FS_IRQHandler()
{
    HAL_PCD_IRQHandler(static_cast<PCD_HandleTypeDef *>(VCP::handlePCD)); //-V2571
}


// Флешка
void OTG_HS_IRQHandler()
{
    HAL_HCD_IRQHandler(reinterpret_cast<HCD_HandleTypeDef *>(HAL_HCD::handleHCD)); //-V2571
}

INTERRUPT_END
