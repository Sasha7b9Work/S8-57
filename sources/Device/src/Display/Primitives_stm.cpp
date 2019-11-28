#include "defines.h"
#include "common/Command.h"
#include "common/Transceiver.h"
#include "Display/Primitives.h"
#include "Utils/Buffer.h"
#include <cstring>


void Region::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[7] =
    {
        Command::Paint_FillRegion,
        static_cast<uint8>(x),
        static_cast<uint8>(x >> 8),
        static_cast<uint8>(y),
        static_cast<uint8>(width),
        static_cast<uint8>(width >> 8),
        static_cast<uint8>(height)
    };

    Transceiver::Send(buffer, 7);
}


void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[7] =
    {
        Command::Paint_DrawRectangle,
        static_cast<uint8>(x),
        static_cast<uint8>(x >> 8),
        static_cast<uint8>(y),
        static_cast<uint8>(width),
        static_cast<uint8>(width >> 8),
        static_cast<uint8>(height)
    };

    Transceiver::Send(buffer, 7);
}


void HLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    int x0 = x;
    int x1 = x0 + width;

    uint8 buffer[6] =
    {
        Command::Paint_DrawHLine,
        static_cast<uint8>(y),
        static_cast<uint8>(x0),
        static_cast<uint8>(x0 >> 8),
        static_cast<uint8>(x1),
        static_cast<uint8>(x1 >> 8)
    };

    Transceiver::Send(buffer, 6);
}


void VLine::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    int y0 = y;
    int y1 = y0 + height;

    uint8 buffer[5] =
    {
        Command::Paint_DrawVLine,
        static_cast<uint8>(x),
        static_cast<uint8>(x >> 8),
        static_cast<uint8>(y0),
        static_cast<uint8>(y1)
    };

    Transceiver::Send(buffer, 5);
}


void Pixel::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[4] =
    {
        Command::Paint_SetPoint,
        static_cast<uint8>(x),
        static_cast<uint8>(x >> 8),
        static_cast<uint8>(y)
    };

    Transceiver::Send(buffer, 4);
}


void Line::Draw(Color color)
{
    color.SetAsCurrent();

    uint8 buffer[7] =
    {
        Command::Paint_DrawLine,
        static_cast<uint8>(x0),
        static_cast<uint8>(x0 >> 8),
        static_cast<uint8>(y0),
        static_cast<uint8>(x1),
        static_cast<uint8>(x1 >> 8),
        static_cast<uint8>(y1)
    };

    Transceiver::Send(buffer, 7);
}


int Text::DrawSmall(int x, int y, Color color)
{
    color.SetAsCurrent();

    uint sizeBuffer = 1 + 2 + 1 + 1 + std::strlen(text);

    Buffer buffer(sizeBuffer);
    buffer.data[0] = Command::Paint_DrawText;
    buffer.data[1] = static_cast<uint8>(x);
    buffer.data[2] = static_cast<uint8>(x >> 8);
    buffer.data[3] = static_cast<uint8>(y);
    buffer.data[4] = static_cast<uint8>(std::strlen(text)); //-V1029

    std::memcpy(&buffer.data[5], static_cast<void *>(const_cast<char *>(text)), std::strlen(text));

    Transceiver::Send(buffer.data, sizeBuffer);

    return x + Font::GetLengthText(text) + 1;
}


void MultiHPointLine::Draw(int x, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[6] =
    {
        Command::Paint_HPointLine,
        0,
        0,
        0,
        static_cast<uint8>(delta),
        static_cast<uint8>(count)
    };

    for (int i = 0; i < numLines; i++)
    {
        buffer[1] = static_cast<uint8>(x);
        buffer[2] = static_cast<uint8>(x >> 8);
        buffer[3] = y[i];

        Transceiver::Send(buffer, 6);
    }
}


void MultiVPointLine::Draw(int y0, Color color)
{
    color.SetAsCurrent();

    uint8 buffer[6] =
    {
        Command::Paint_VPointLine,
        0,
        0,
        0,
        static_cast<uint8>(delta),
        static_cast<uint8>(count)
    };

    for (int i = 0; i < numLines; i++)
    {
        int x = x0[i];

        buffer[1] = static_cast<uint8>(x);
        buffer[2] = static_cast<uint8>(x >> 8);
        buffer[3] = static_cast<uint8>(y0);

        Transceiver::Send(buffer, 6);
    }
}
