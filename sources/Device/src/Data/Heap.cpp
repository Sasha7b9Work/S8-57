#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Heap.h"
#include "Menu/Pages/Include/PageMemory.h"


static const uint SIZE_BUFFER = 112 * 1024;
static uint8 buffer[SIZE_BUFFER];

static const uint SIZE_STORAGE_RAM = SIZE_BUFFER;


uint8 *Heap::BeginStorageRAM()
{
    return buffer;
}


uint8 *Heap::EndStorageRAM()
{
    return BeginStorageRAM() + SIZE_STORAGE_RAM;
}
