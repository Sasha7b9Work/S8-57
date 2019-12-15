#include "defines.h"
#include "common/Transceiver.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Hardware/CPU.h"
#include "Utils/DecoderPanel.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"
#include <cstdlib>
#include <cstdio>


int drawSymbol = 10;

int SL = -1;
int RL = -1;


using std::rand;

int main()
{
    CPU::Init();
    Display::Init();
    Keyboard::Init();
    Transceiver::Init();


    HAL_DAC2::SetValue(255);

    while(1)
    {
        static uint prevTime = 0;
        
        Transceiver::Receive();

        if(TIME_MS - prevTime > 2)
        {
            Keyboard::Update();
            prevTime = TIME_MS;
        }

        Painter::BeginScene();

        Painter::SetColor(Color::WHITE);

        Painter::FillRegion(0, 0, 200, 200);

        Painter::SetColor(Color::BLACK);

        char buffer[100];

        std::sprintf(buffer, "%d rl = %d sl = %d", drawSymbol, RL, SL);

        Text::Draw(10, 10, buffer);

        Painter::EndScene();

        drawSymbol = 0;
    }
}
