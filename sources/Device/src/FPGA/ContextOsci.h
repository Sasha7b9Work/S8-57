#pragma once
#include "FPGA/TypesFPGA.h"
#include "FPGA/MathFPGA.h"


struct ContextOsci
{
    static void OnPressStart();

    static uint &timeStart;
};
