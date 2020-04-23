#pragma once
#include "stm32f437xx.h"


typedef enum
{
    DISABLE = 0U,
    ENABLE = !DISABLE //-V2570
} FunctionalState;
