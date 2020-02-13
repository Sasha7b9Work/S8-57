#include "defines.h"
#include "common/Decoder_d.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"
#include <stdlib.h>



int main()
{
    CPU::Init();
    Display::Init();
    Keyboard::Init();

    while(1)
    {
        static uint prevTime = 0;

        HAL_BUS::Update();

        if(TIME_MS - prevTime > 2)
        {
            Keyboard::Update();
            prevTime = TIME_MS;

//            static int delta = -1;
//            static int distance = 0;
//            static int counter = 0;
//            counter++;
//
//            if((counter % 10) == 0)
//            {
//                Keyboard::SendCommand(delta < 0 ? Control::B_RShiftLessA : Control::B_RShiftMoreA, Control::Action::Press);
//                for(int i = 0; i < 16; i++)
//                {
//                    Keyboard::SendCommand(delta < 0 ? Control::B_RShiftLessA : Control::B_RShiftMoreA, Control::Action::Repeat);
//                }
//                Keyboard::SendCommand(delta < 0 ? Control::B_RShiftLessA : Control::B_RShiftMoreA, Control::Action::Release);
//            
//                distance += delta;
//            
//                if(distance < -50)
//                {
//                    delta = 1;
//                }
//                else if(distance > 50)
//                {
//                    delta = -1;
//                }
//            }
        }
    }
}
