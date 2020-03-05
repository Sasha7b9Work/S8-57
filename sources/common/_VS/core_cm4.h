#pragma once

#define __CC_ARM

#include "stdint.h"

#undef __ASM
#define __ASM()


struct SysTickStruct
{
    unsigned short CTRL;
};

#define SysTick    ((SysTickStruct *)0)

#define NVIC_SetPriority(x, y)

inline void NVIC_SystemReset(void) {};


#define SCB_SHCSR_MEMFAULTENA_Msk 0
#define MPU_CTRL_ENABLE_Msk 0


extern struct structSCB
{
    unsigned VTOR;
    unsigned SHCSR;
} *SCB;


extern struct structMPU
{
    unsigned CTRL;
} *MPU;

