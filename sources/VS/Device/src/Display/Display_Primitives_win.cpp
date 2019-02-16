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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::Rectangle::Draw(int x, int y, Color color)
{
    Color::SetCurrent(color);
    SDL_Rect rect = { x, y, width + 1, height + 1 };
    SDL_RenderDrawRect(renderer, &rect);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::HLine::Draw(int x, int y, Color color)
{
    Color::SetCurrent(color);
    SDL_Rect rect = {x, y, width, 1};
    SDL_RenderFillRect(renderer, &rect);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::VLine::Draw(int x, int y, Color color)
{
    Color::SetCurrent(color);
    SDL_Rect rect = {x, y, 1, height};
    SDL_RenderFillRect(renderer, &rect);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::Point::Draw(int x, int y, Color color)
{
    Color::SetCurrent(color);
    SDL_Rect rect = { x, y, 1, 1 };
    SDL_RenderFillRect(renderer, &rect);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::Line::Draw(Color color)
{
    Color::SetCurrent(color);
    SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
}
