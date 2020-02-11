#include "defines.h"
#include "log.h"
#include "Hardware/Beeper.h"
#include "FlashDrive/FlashDrive.h"      
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include "Utils/Debug.h"
#include "Display/Painter.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


INTERRUPT_BEGIN



void HardFault_Handler()
{
    __IO const char *file = Debug::file;
    __IO int line = Debug::line;
    
    LOG_WRITE_AND_SHOW("%s %d", file, line);

    while (1)           // -V776
    {
        file = file;    // -V570
        line = line;    // -V570
    }
}


void TIM3_IRQHandler()
{
    if ((TIM3->SR & TIM_SR_UIF) == TIM_SR_UIF)
    {
        if ((TIM3->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)
        {
            TIM3->SR = ~TIM_DIER_UIE;
            Timer::ElapsedCallback();
        }
    }
}


void SysTick_Handler(void)
{
    HAL_IncTick();
}


void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);       // Тестер компонентов
}


// VCP
void OTG_FS_IRQHandler()
{
    HAL_PCD_IRQHandler(static_cast<PCD_HandleTypeDef *>(VCP::HandlePCD()));
}


// Флешка
void OTG_HS_IRQHandler()
{
    HAL_HCD_IRQHandler(&FDrive::handleHCD);
}



void NMI_Handler(void)
{
}


void MemManage_Handler()
{
    while (1)
    {
    }
}


void BusFault_Handler()
{
    while (1)
    {
    }
}


void UsageFault_Handler()
{
    while (1)
    {
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

INTERRUPT_END
