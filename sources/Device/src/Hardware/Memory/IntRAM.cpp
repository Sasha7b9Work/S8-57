#include "defines.h"
#include "Hardware/Memory/IntRAM.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Osci/DataSettings.h"


static const uint SIZE_BUFFER = 112 * 1024;
static uint8 buffer[SIZE_BUFFER];

static const uint SIZE_STORAGE_RAM = SIZE_BUFFER;


uint8 *IntRAM::Begin()
{
    return buffer;
}


uint8 *IntRAM::End()
{
    return Begin() + SIZE_STORAGE_RAM;
}
