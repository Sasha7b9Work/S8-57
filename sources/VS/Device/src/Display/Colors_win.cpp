#include "defines.h"
#include "Display/Colors.h"
#include <SDL.h>


extern SDL_Renderer *renderer;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Color::SetCurrent(Color color)
{
    if (color != Color::NUMBER)
    {
        uint value = COLOR(color.value);
        uint8 blue = (uint8)value;
        uint8 green = (uint8)(value >> 8);
        uint8 red = (uint8)(value >> 16);
        SDL_SetRenderDrawColor(renderer, red, green, blue, 0x00);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Color::SetAsCurrent()
{
    if (value != Color::NUMBER.value)
    {
        uint color = COLOR(value);
        uint8 blue = (uint8)color;
        uint8 green = (uint8)(color >> 8);
        uint8 red = (uint8)(color >> 16);
        SDL_SetRenderDrawColor(renderer, red, green, blue, 0x00);
    }
}
