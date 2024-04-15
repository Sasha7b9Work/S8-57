// 2024/04/15 08:34:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "Buffer.h"
#include "Hardware/VCP.h"
#include <cstdlib>
#include <cstring>
#include <cstdio>


Buffer::Buffer(int s)
{
    Malloc(s);
}


Buffer::~Buffer()
{
    Free();
}


void Buffer::Realloc(int _size)
{
    Free();
    Malloc(_size);
}


void Buffer::Fill(uint8 value)
{
    if (size)
    {
        std::memset(data, value, static_cast<uint>(size));
    }
}


void Buffer::Free()
{
    std::free(data);
    data = nullptr;
    size = 0U;
}


void Buffer::Malloc(int s)
{
    if (s > 0)
    {
        data = static_cast<uint8 *>(std::malloc(static_cast<uint>(s)));
        size = (data) ? s : 0;

        if(!data)
        {
            LOG_ERROR("Нет памяти");
        }
    }
    else
    {
        data = nullptr;
        size = 0U;
    }
}


void Buffer::Log()
{
    LogU8(data);
}


void Buffer::LogU8(const uint8 *buffer)
{
    LOG_WRITE("%d %d %d %d %d %d %d %d %d %d", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9]);
}


void Buffer::LogU8(const uint8 *buffer, int size)
{
    char message[256] = { '\0' };

    for (int i = 0; i < size; i++)
    {
        char value[8];

        std::sprintf(value, "%u ", buffer[i]);

        std::strcat(message, value);
    }

    std::strcat(message, "\r\n");

    VCP::SendStringAsynch(message);
}
