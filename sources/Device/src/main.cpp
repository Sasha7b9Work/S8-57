#include "defines.h"
#include "device.h"
#include "Hardware/AT25160N.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Hardware/Memory/ExtRAM.h"
#include "Test/Test.h"


int main(void)
{
    Device::Init();

    Timer::PauseOnTime(1);


    S_SERV_ENUM_VOLUME = 3;

    while (true)
    {
        Beeper::Beep(TypePress::Press);
        Beeper::Beep(TypePress::Release);
    }

//    AT25160N::Init();

//    AT25160N::Test();
    
//    Test::Run();
  
    while (1)
    {
        Device::Update();
    }
}
