#pragma once
#include "defines.h"
#include "Hardware/stm32/stm746.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CPU : public STM746
{
public:

    static void Init();

    //---------------------------------------------------------------------------------------------------------------------------------------- FMC ---
    class FMC_
    {
        friend class CPU;
    private:
        static void Init();
    };

    //-------------------------------------------------------------------------------------------------------------------------------------- SDRAM ---
    class SDRAM_
    {
        friend class CPU;
    private:
        static void Init();
        static void InitializationSequence(uint count);
        static SDRAM_HandleTypeDef sdramHandle;
#define SDRAM_DEVICE_ADDR ((uint)0xD0000000)
    };

private:

    static void SystemClockConfig();
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C" {
#endif
    
    /// Timer.cpp
    void TIM3_IRQHandler();
    void TIM4_IRQHAndler();
    
    void SysTick_Handler();
    void MemManage_Handler();
    void UsageFault_Handler();
    void PendSV_Handler();
    void NMI_Handler();
    void HardFault_Handler();
    void BusFault_Handler();
    void SVC_Handler();
    void DebugMon_Handler();

#ifdef __cplusplus
}
#endif
