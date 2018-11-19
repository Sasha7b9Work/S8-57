#include "Buffer.h"
#include <stdlib.h>


Buffer::Buffer(uint s)
{
    buffer = (uint8 *)malloc(s);

    size = buffer ? s : 0U;
}

Buffer::~Buffer()
{
    free(buffer);
}

uint8 *Buffer::Data()
{
    return buffer;
}

uint Buffer::Size() const
{
    return size;
}
