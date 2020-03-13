#include "HAL.h"



void HAL::ErrorHandler(const char *, int)
{
    while (true) {}
}


void HAL::Init()
{
    HAL_BUS::Init();
}
