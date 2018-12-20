#pragma once

typedef struct
{
    int i;
} HCD_HandleTypeDef;

void HAL_HCD_IRQHandler(HCD_HandleTypeDef *hhcd);
