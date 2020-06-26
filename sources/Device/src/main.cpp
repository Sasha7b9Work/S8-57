#include "defines.h"
#include "device.h"
#include "Hardware/AT25160N.h"
#include "Hardware/Timer.h"
#include "Hardware/Memory/ExtRAM.h"
#include "Test/Test.h"


int main(void)
{
    Device::Init();

    Timer::PauseOnTime(1000);

//    AT25160N::Init();

//    AT25160N::Test();
    
//    Test::Run();
  
    while (1)
    {
        Device::Update();
    }
}
