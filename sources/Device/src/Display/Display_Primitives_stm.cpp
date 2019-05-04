#include "defines.h"
#include "Command.h"
#include "Transceiver.h"
#include "Display/Display_Primitives.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>
#include "Utils/Buffer.h"
#include "Display/Font/Font.h"

using HAL::FSMC;

using namespace Transceiver;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Primitives::Region::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();
    uint8 buffer[7] = { Command::Paint_FillRegion, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    Transmitter::Send(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    uint8 buffer[7] = { Command::Paint_DrawRectangle, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    Transmitter::Send(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    int x0 = x;
    int x1 = x0 + width;
    uint8 buffer[6] = { Command::Paint_DrawHLine, (uint8)y, (uint8)x0, (uint8)(x0 >> 8), (uint8)x1, (uint8)(x1 >> 8) };
    Transmitter::Send(buffer, 6);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    int y0 = y;
    int y1 = y0 + height;
    uint8 buffer[5] = { Command::Paint_DrawVLine, (uint8)x, (uint8)(x >> 8), (uint8)y0, (uint8)y1 };
    Transmitter::Send(buffer, 5);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::Point::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    uint8 buffer[4] = { Command::Paint_SetPoint, (uint8)x, (uint8)(x >> 8), (uint8)y };
    Transmitter::Send(buffer, 4);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::Line::Draw(Color color)
{
    color.SetAsCurrent();
    uint8 buffer[7] = { Command::Paint_DrawLine, (uint8)x0, (uint8)(x0 >> 8), (uint8)y0, (uint8)x1, (uint8)(x1 >> 8), (uint8)y1 };
    Transmitter::Send(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::Primitives::Text::DrawSmall(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint sizeBuffer = 1 + 2 + 1 + 1 + std::strlen(text);

    Buffer buffer(sizeBuffer);
    buffer.data[0] = Command::Paint_DrawText;
    buffer.data[1] = (uint8)x;
    buffer.data[2] = (uint8)(x >> 8);
    buffer.data[3] = (uint8)y;
    buffer.data[4] = (uint8)std::strlen(text); //-V1029

    std::memcpy(&buffer.data[5], (void *)text, std::strlen(text));

    Transmitter::Send(buffer.data, sizeBuffer);

    return x + Font::GetLengthText(text) + 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::Text::DrawBig(int x, int y, Color color)
{
#define MAX_SIZE_BUFFER 100

    color.SetAsCurrent();

    uint numSymbols = std::strlen(text); //-V2513
    uint8 buffer[MAX_SIZE_BUFFER] = { Command::Paint_DrawBigText, (uint8)x, (uint8)(x >> 8), (uint8)y, sizeOfType, (uint8)(numSymbols) };

    uint8 *pointer = &buffer[6];

    while (*text)
    {
        *pointer++ = (uint8)*text++;
    }

    Transmitter::Send(buffer, 1 + 2 + 1 + 1 + numSymbols + 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::MultiHPointLine::Draw(int x, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[6] = { Command::Paint_HPointLine, 0, 0, 0, (uint8)delta, (uint8)count };

    for (int i = 0; i < numLines; i++)
    {
        buffer[1] = (uint8)x;
        buffer[2] = (uint8)(x >> 8);
        buffer[3] = y[i];

        Transmitter::Send(buffer, 6);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Primitives::MultiVPointLine::Draw(int y0, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[6] = { Command::Paint_VPointLine, 0, 0, 0, (uint8)delta, (uint8)count };

    for (int i = 0; i < numLines; i++)
    {
        int x = x0[i];

        buffer[1] = (uint8)x;
        buffer[2] = (uint8)(x >> 8);
        buffer[3] = (uint8)y0;

        Transmitter::Send(buffer, 6);
    }
}
