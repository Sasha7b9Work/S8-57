#include "defines.h"
#include "common/Command.h"
#include "common/Transceiver.h"
#include "Display/Colors.h"
#include "Recorder/Recorder.h"
#include <SDL.h>


extern SDL_Renderer *renderer;


void Transceiver::Init()
{

}


void Transceiver::Send(const uint8 *data, uint num)
{
    if(num == 2 && *data == Command::Paint_SetColor)
    {
        uint color = COLOR(data[1]);
        uint8 blue = static_cast<uint8>(color);
        uint8 green = static_cast<uint8>(color >> 8);
        uint8 red = static_cast<uint8>(color >> 16);
        SDL_SetRenderDrawColor(renderer, red, green, blue, 0x00);
    }
    else
    {
        Recorder::ReadPoint();
    }
}


void Transceiver::Send(uint8)
{

}


bool Transceiver::Receive()
{
    return false;
}
