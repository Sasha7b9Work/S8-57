#include "defines.h"
#include "device.h"
#include "Hardware/AT25160N.h"
#include "Hardware/Memory/ExtRAM.h"
#include "Test/Test.h"


int main(void)
{
    Device::Init();

    AT25160N::Init();
    
    Test::Run();
  
    ExtRAM::Test4::Prepare();

    while (1)
    {
        Device::Update();

        //LOG_WRITE("test %f", ExtRAM::TestTime(512) * 1000);
    }
}
