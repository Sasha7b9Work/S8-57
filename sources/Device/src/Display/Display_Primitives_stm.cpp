#include "defines.h"
#include "Command.h"
#include "Display/Display_Primitives.h"
#include "Hardware/HAL/HAL.h"


using HAL::FSMC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Primitives::Region::Fill(int x, int y, Color color)
{
    Color::SetCurrent(color);
    uint8 buffer[7] = { Command::Paint_FillRegion, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    FSMC::WriteToPanel(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::Rectangle::Draw(int x, int y, Color color)
{
    Color::SetCurrent(color);
    uint8 buffer[7] = { Command::Paint_DrawRectangle, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    FSMC::WriteToPanel(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::HLine::Draw(int x, int y, Color color)
{
    Color::SetCurrent(color);
    int x0 = x;
    int x1 = x0 + width;
    uint8 buffer[6] = { Command::Paint_DrawHLine, (uint8)y, (uint8)x0, (uint8)(x0 >> 8), (uint8)x1, (uint8)(x1 >> 8) };
    FSMC::WriteToPanel(buffer, 6);
}
