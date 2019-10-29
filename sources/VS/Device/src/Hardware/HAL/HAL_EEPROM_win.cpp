#include "defines.h"
#include "Hardware/Memory/Memory.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


#define  _16K (16 * 1024)
#define  _64K (64 * 1024)
#define _128K (128 * 1024)


struct SectorTypeDef
{
	uint address;
	uint8* memory;
	uint size;
	/// ¬озвращает true, если сектор содержит €чейку пам€ти с адресом addr
	bool IsConsist(uint addr) const
	{
		return (addr >= reinterpret_cast<uint>(memory)) && (addr < reinterpret_cast<uint>(memory + size));
	}

	void WriteByte(uint addr, uint8 value) const
	{
		if (IsConsist(addr))
		{
			*(reinterpret_cast<uint8*>(addr)) = value;
		}
	}
};


uint8 eeprom[EEPROM::SIZE];


static const SectorTypeDef sectors[24] =
{
	{S00_BOOT_1,     &eeprom[0],                     _16K},
	{S01_BOOT_2,     &eeprom[_16K],                  _16K},
	{S02,            &eeprom[_16K * 2],              _16K},
	{S03,            &eeprom[_16K * 3],              _16K},
	{S04,            &eeprom[_16K * 4],              _64K},
	{S05_FIRM_1,     &eeprom[_128K],                 _128K},
	{S06_FIRM_2,     &eeprom[_128K * 2],             _128K},
	{S07_FIRM_3,     &eeprom[_128K * 3],             _128K},
	{S08,            &eeprom[_128K * 4],             _128K},
	{S09,            &eeprom[_128K * 5],             _128K},
	{S10_SETTINGS_1, &eeprom[_128K * 6],             _128K},
	{S11_SETTINGS_2, &eeprom[_128K * 7],             _128K},
	{S12,            &eeprom[_128K * 8],             _16K},
	{S13,            &eeprom[_128K * 8 + _16K],      _16K},
	{S14,            &eeprom[_128K * 8 + _16K * 2],  _16K},
	{S15,            &eeprom[_128K * 8 + _16K * 3],  _16K},
	{S16,            &eeprom[_128K * 8 + _16K * 4],  _64K},
	{S17_RECORDER_1, &eeprom[_128K * 8 + _128K],     _128K},
	{S18_RECORDER_2, &eeprom[_128K * 8 + _128K * 2], _128K},
	{S19,            &eeprom[_128K * 8 + _128K * 3], _128K},
	{S20,            &eeprom[_128K * 8 + _128K * 4], _128K},
	{S21,            &eeprom[_128K * 8 + _128K * 5], _128K},
	{S22,            &eeprom[_128K * 8 + _128K * 6], _128K},
	{S23,            &eeprom[_128K * 8 + _128K * 7], _128K}
};


/// ¬озвращает ссылку на структуру описани€ сектора с начальным адресом address
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