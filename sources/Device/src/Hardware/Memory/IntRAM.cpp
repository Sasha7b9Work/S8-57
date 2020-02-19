#include "defines.h"
#include "Hardware/Memory/IntRAM.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Osci/DataSettings.h"


/*
    Смещение  | Использование памяти
    от начала |
       0      | усреднение 1-го канала
       16k    | усреднение 2-го канала
       32k    | чтение рандомизатора 1-го канала
       40k    | чтение нандомизатора 2-го канала
       48k    |
*/


static const uint SIZE_BUFFER = 112 * 1024;
static uint8 buffer[SIZE_BUFFER];

static uint16 *const ave[2] = { reinterpret_cast<uint16 *>(buffer), reinterpret_cast<uint16 *>(buffer + 16 * 1024) };
static uint8 *const rand[2] = { buffer + 32 * 1024, buffer + 40 * 1024 };


uint16 *IntRAM::Averager16k(Chan::E ch)
{
    return ave[ch];
}


uint8 *IntRAM::ReadRand(Chan::E ch)
{
    return rand[ch];
}
