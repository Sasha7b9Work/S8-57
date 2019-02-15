#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"

#include <SDL.h>


extern SDL_Renderer *renderer;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Primitives::Region::Fill(int x, int y, Color color)
{
    Color::SetCurrent(color);

    SDL_Rect rect = { x, y, width + 1, height + 1 };

    SDL_RenderFillRect(renderer, &rect);
}
