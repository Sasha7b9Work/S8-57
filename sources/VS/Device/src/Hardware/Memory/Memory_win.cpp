#include "defines.h"
#include "Hardware/Memory/MemoryROM.h"
#include <cstring>
#include <SDL.h>


#define FILE_NAME "memory.bin"


extern uint8 eeprom[FlashMemory::SIZE];


void MemorySave()
{
    SDL_RWops *file = SDL_RWFromFile(FILE_NAME, "wb");

    if (file)
    {
        SDL_RWseek(file, 0, RW_SEEK_SET);
        SDL_RWwrite(file, eeprom, 1, FlashMemory::SIZE);
        SDL_RWclose(file);
    }
}


void MemoryLoad()
{
    SDL_RWops *file = SDL_RWFromFile(FILE_NAME, "rb");

    if (file)
    {
        SDL_RWread(file, eeprom, 1, FlashMemory::SIZE);
        SDL_RWclose(file);
    }
	else
	{
		std::memset(eeprom, 0xFF, FlashMemory::SIZE);
	}
}
