#include "defines.h"
#include "device.h"
#include "Hardware/AT25160N.h"
#include "Hardware/HAL/HAL.h"
#include "Test/Test.h"


int main(void)
{
    Device::Init();

    AT25160N::Init();
    
    Test::Run();
  
    while (1)
    {
        //Device::Update();

        HAL_FSMC::TestRAM1();
    }
}
