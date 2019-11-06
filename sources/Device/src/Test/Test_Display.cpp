#include "defines.h"
#include "Test/Test.h"
#include "Hardware/HAL/HAL.h"


static void Update()
{

}


void Test::Display::Init()
{
    HAL_TIM4::Init(100, Update);
}


void Test::Display::DeInit()
{
    HAL_TIM4::DeInit();
}
