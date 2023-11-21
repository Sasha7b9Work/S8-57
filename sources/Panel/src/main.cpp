// 2023/11/21 08:28:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Decoder_d.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"
#include <stdlib.h>


static void UpdateDisplay()
{
    static int x = 0;
    static int y = 0;
    static int width = 10;
    static int dx = 1;
    static int dy = 0;

    Painter::SetColor(Color::BLACK);

    Painter::BeginScene();

    Painter::SetColor(Color::WHITE);

    Painter::FillRegion(x, y, width, width);

    x += dx;
    y += dy;

    if (x + width > Display::WIDTH)
    {
        x--;
        dx = 0;
        dy = 1;
    }
    else if (y + width > Display::HEIGHT)
    {
        y--;
        dx = -1;
        dy = 0;
    }
    else if (x < 0)
    {
        x++;
        dx = 0;
        dy = -1;
    }
    else if (y < 0)
    {
        y++;
        dx = 1;
        dy = 0;
    }

    Painter::EndScene();
}


int main()
{
    CPU::Init();
    Display::Init();
    Keyboard::Init();

    Painter::SetColorValue(Color::WHITE, 0xFFFFFFFF);
    Painter::SetColorValue(Color::BLACK, 0x00000000);

    Painter::LoadPalette();

    HAL_DAC2::SetValue(255);

    while(1)
    {
        if (!HAL_BUS::DataIsReceived())
        {
            UpdateDisplay();
        }

        static uint prevTime = 0;

        HAL_BUS::Update();

        if(TIME_MS - prevTime > 2)
        {
            Keyboard::Update();
            prevTime = TIME_MS;
        }
    }
}
