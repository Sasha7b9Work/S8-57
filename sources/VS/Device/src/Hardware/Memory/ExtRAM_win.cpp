#include "defines.h"
#include "Hardware/Memory/ExtRAM.h"


#define SIZE_BUFFER (512 * 1024)


static uint8 buffer[SIZE_BUFFER];


uint8 *ExtRAM::Begin()
{
    return buffer;
}


uint8 *ExtRAM::End()
{
    return buffer + SIZE_BUFFER;
}

