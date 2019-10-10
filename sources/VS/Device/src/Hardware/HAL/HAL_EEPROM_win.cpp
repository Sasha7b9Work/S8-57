#include "defines.h"
#include "Hardware/Memory.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


/// Возвращает ссылку на структуру описания сектора с начальным адресом address
static const SectorTypeDef &GetSector(uint address)
{
    for (int i = 0; i < 24; i++)
    {
        const SectorTypeDef &sector = sectors[i];

        if (sector.IsConsist(address))
        {
            return sector;
        }
    }

    return sectors[-1];
}


void HAL_EEPROM::WriteBufferBytes(uint address, void *buffer, int size)
{
    WriteBytes(address, (const uint8 *)buffer, size);
}


void HAL_EEPROM::EraseSector(uint address)
{
    const SectorTypeDef &sector = GetSector(address);

    std::memset(sector.memory, 0xFF, sector.size);
}


void HAL_EEPROM::WriteBytes(uint address, const uint8 *buffer, int size)
{
    uint8 *data = (uint8 *)buffer;

    for (int i = 0; i < size; i++)
    {
        const SectorTypeDef &sector = GetSector(address);

        sector.WriteByte(address++, *data++);
    }
}