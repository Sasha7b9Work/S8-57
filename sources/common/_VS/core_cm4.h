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


extern struct structSCB
{
    unsigned VTOR;
} *SCB;

