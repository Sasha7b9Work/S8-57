#include "defines.h"
#include "Hardware/Memory.h"
#include <SDL.h>


#define FILE_NAME "memory.bin"


extern uint8 eeprom[EEPROM::SIZE];


void MemorySave()
{
    SDL_RWops *file = SDL_RWFromFile(FILE_NAME, "wb");
    if (file != nullptr)
    {
        SDL_RWseek(file, 0, RW_SEEK_SET);
        SDL_RWwrite(file, eeprom, 1, EEPROM::SIZE);
        SDL_RWclose(file);
    }
}


void MemoryLoad()
{
    SDL_RWops *file = SDL_RWFromFile(FILE_NAME, "rb");
    if (file != nullptr)
    {
        SDL_RWread(file, eeprom, 1, EEPROM::SIZE);
        SDL_RWclose(file);
    }
}
