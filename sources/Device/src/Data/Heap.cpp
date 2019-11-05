#include "defines.h"
#include "Data/Heap.h"



static const uint SIZE_BUFFER = 79 * 1024;
static uint8 buffer[SIZE_BUFFER];



uint8 *Heap::Begin()
{
    return buffer;
}


uint8 *Heap::End()
{
    return buffer + Size();
}


uint Heap::Size()
{
    return SIZE_BUFFER;
}
