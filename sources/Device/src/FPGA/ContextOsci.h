#pragma once
#include "FPGA/TypesFPGA.h"
#include "FPGA/MathFPGA.h"


struct ContextOsci
{
    static void OnPressStart();

    static uint BytesInChannel();

    static bool InStateStop();

    static bool InStateWait();

    static uint16 &post;
    static uint16 &pred;
    static uint &timeStart;
    static bool &isRunning;
};
