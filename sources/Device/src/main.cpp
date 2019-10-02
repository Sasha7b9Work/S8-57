#include "defines.h"
#include "Device.h"
#include "Hardware/AT25160N.h"
#include "Hardware/Timer.h"


//////////
int main(void)
{
    Device::Init();

    AT25160N::Init();

    while (1)
    {
        Device::Update();       
    }

}
