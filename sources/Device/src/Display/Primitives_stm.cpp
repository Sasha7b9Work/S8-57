#include "defines.h"
#include "Command.h"
#include "Transceiver.h"
#include "Display/Primitives.h"
#include "Utils/Buffer.h"
#include <cstring>


void Region::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();
    uint8 buffer[7] = { Command::Paint_FillRegion, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    Transceiver::Send(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    uint8 buffer[7] = { Command::Paint_DrawRectangle, (uint8)x, (uint8)(x >> 8), (uint8)y, (uint8)width, (uint8)(width >> 8), (uint8)height };
    Transceiver::Send(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    int x0 = x;
    int x1 = x0 + width;
    uint8 buffer[6] = { Command::Paint_DrawHLine, (uint8)y, (uint8)x0, (uint8)(x0 >> 8), (uint8)x1, (uint8)(x1 >> 8) };
    Transceiver::Send(buffer, 6);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    int y0 = y;
    int y1 = y0 + height;
    uint8 buffer[5] = { Command::Paint_DrawVLine, (uint8)x, (uint8)(x >> 8), (uint8)y0, (uint8)y1 };
    Transceiver::Send(buffer, 5);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Pixel::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();
    uint8 buffer[4] = { Command::Paint_SetPoint, (uint8)x, (uint8)(x >> 8), (uint8)y };
    Transceiver::Send(buffer, 4);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Line::Draw(Color color)
{
    color.SetAsCurrent();
    uint8 buffer[7] = { Command::Paint_DrawLine, (uint8)x0, (uint8)(x0 >> 8), (uint8)y0, (uint8)x1, (uint8)(x1 >> 8), (uint8)y1 };
    Transceiver::Send(buffer, 7);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Text::DrawSmall(int x, int y, Color color)
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

    Transceiver::Send(buffer.data, sizeBuffer);

    return x + Font::GetLengthText(text) + 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Text::DrawBig(int x, int y, Color color)
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

    Transceiver::Send(buffer, 1 + 2 + 1 + 1 + numSymbols + 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MultiHPointLine::Draw(int x, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[6] = { Command::Paint_HPointLine, 0, 0, 0, (uint8)delta, (uint8)count };

    for (int i = 0; i < numLines; i++)
    {
        buffer[1] = (uint8)x;
        buffer[2] = (uint8)(x >> 8);
        buffer[3] = y[i];

        Transceiver::Send(buffer, 6);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void MultiVPointLine::Draw(int y0, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[6] = { Command::Paint_VPointLine, 0, 0, 0, (uint8)delta, (uint8)count };

    for (int i = 0; i < numLines; i++)
    {
        int x = x0[i];

        buffer[1] = (uint8)x;
        buffer[2] = (uint8)(x >> 8);
        buffer[3] = (uint8)y0;

        Transceiver::Send(buffer, 6);
    }
}
