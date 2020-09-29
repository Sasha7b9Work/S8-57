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

    HAL_DAC2::SetValue(100);

    Painter::SetColorValue(Color::WHITE, 0x00000000);
    Painter::SetColorValue(Color::BLACK, 0x00ffffff);

    int x = 0;

    while(1)
    {
        Painter::SetColor(Color::WHITE);
        Painter::BeginScene();
        Painter::SetColor(Color::BLACK);
        Painter::FillRegion(x, x++, 100, 100);
        Painter::EndScene();
        if (x > 100)
        {
            x = 0;
        }

//        static uint prevTime = 0;
//
//        HAL_BUS::Update();
//
//        if(TIME_MS - prevTime > 2)
//        {
//            Keyboard::Update();
//            prevTime = TIME_MS;
//        }
    }
}
