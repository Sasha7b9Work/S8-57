#include "defines.h"
#include "Device.h"
#include "Hardware/AT25160N.h"
#include "Hardware/Timer.h"
#include "Test/Test.h"


int main(void)
{
    Device::Init();

    AT25160N::Init();

    //if (set.dbg.runTest)
    {
        set.dbg.runTest = false;
        Test::Run();
    }

    while (1)
    {
        Device::Update();       
    }

}
