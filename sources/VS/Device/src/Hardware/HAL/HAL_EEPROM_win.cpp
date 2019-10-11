#include "defines.h"
#include "Hardware/Memory.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


/// Возвращает ссылку на структуру описания сектора с начальным адресом address
static const SectorTypeDef &GetSector(uint address)
{
    const SectorTypeDef *result = &sectors[0];

    for (int i = 0; i < 24; i++)
    {
        const SectorTypeDef &sector = sectors[i];

        if (sector.IsConsist(address))
        {
            result = &sector;
            break;
        }
    }

    return *result;
}


void HAL_EEPROM::WriteBufferBytes(uint address, const void *buffer, int size)
{
    WriteBytes(address, static_cast<const uint8 *>(buffer), size);
}


void HAL_EEPROM::EraseSector(uint address)
{
    const SectorTypeDef &sector = GetSector(address);

    std::memset(sector.memory, 0xFF, sector.size);
}


void HAL_EEPROM::WriteBytes(uint address, const uint8 *buffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        const SectorTypeDef &sector = GetSector(address);

        sector.WriteByte(address++, *buffer++);
    }
}