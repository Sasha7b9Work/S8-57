#include "defines.h"
#include "Data/DataSettings.h"
#include "Data/Heap.h"
#include "Menu/Pages/Include/PageMemory.h"


static const uint SIZE_BUFFER = 79 * 1024;
static uint8 buffer[SIZE_BUFFER];

//                                                                                  Chan::Count Пиковый детектор не учитываем, потому что при пиковом детекторе максимальная длина памяти в точках составляет половину от доступной
static const uint SIZE_FRAME_P2P = sizeof(DataSettings) + ENumPointsFPGA::MAX_NUM *      2;

static const uint SIZE_STORAGE_RAM = SIZE_BUFFER - SIZE_FRAME_P2P;


uint8 *Heap::BeginStorageRAM()
{
    return buffer + SIZE_FRAME_P2P;
}


uint8 *Heap::EndStorageRAM()
{
    return BeginStorageRAM() + SIZE_STORAGE_RAM;
}
