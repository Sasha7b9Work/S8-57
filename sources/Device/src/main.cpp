#include "defines.h"
#include "device.h"
#include "Hardware/AT25160N.h"
#include "Hardware/Memory/ExtRAM.h"
#include "Test/Test.h"


int main(void)
{
    Device::Init();

//    AT25160N::Init();

//    AT25160N::Test();
    
//    Test::Run();
  
    while (1)
    {
        Device::Update();
    }
}
