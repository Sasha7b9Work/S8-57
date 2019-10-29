#include "defines.h"
#include "Hardware/Memory/Memory.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


#define  _16K (16 * 1024)
#define  _64K (64 * 1024)
#define _128K (128 * 1024)


bool operator >=(const Address &left, const Address &right)
{
    return (left.addressMC >= right.addressMC);
}

bool operator<(const Address &left, const Address &right)
{
    return (left.addressMC < right.addressMC);
}


uint8 eeprom[EEPROM::SIZE];


struct SectorTypeDef
{
	Address address;
	uint8*  memory;
	uint    size;
	/// ¬озвращает true, если сектор содержит €чейку пам€ти с адресом addr
	bool IsConsist(Address addr) const
	{
		return (addr >= Address::FromPC(memory)) && (addr < Address::FromPC(memory + size));
	}

	void WriteByte(Address addr, uint8 value) const
	{
		if (IsConsist(addr))
		{
            *addr.ToPC() = value;
		}
	}
};


Address Address::FromPC(uint addr)
{
    uint deltaPC = addr - reinterpret_cast<uint>(&eeprom[0]);

    return Address(S00_BOOT_1 + deltaPC);
}

Address Address::FromMC(uint addr)
{
    return Address(addr);
}

Address Address::FromPC(uint8 *addr)
{
    return FromPC(reinterpret_cast<uint>(addr));
}

uint8 *Address::ToPC() const
{
    uint deltaMC = addressMC - S00_BOOT_1;

    return eeprom + deltaMC;
}


static const SectorTypeDef sectors[24] =
{
	{Address::FromMC(S00_BOOT_1),     &eeprom[0],                     _16K},
	{Address::FromMC(S01_BOOT_2),     &eeprom[_16K],                  _16K},
	{Address::FromMC(S02),            &eeprom[_16K * 2],              _16K},
	{Address::FromMC(S03),            &eeprom[_16K * 3],              _16K},
	{Address::FromMC(S04),            &eeprom[_16K * 4],              _64K},
	{Address::FromMC(S05_FIRM_1),     &eeprom[_128K],                 _128K},
	{Address::FromMC(S06_FIRM_2),     &eeprom[_128K * 2],             _128K},
	{Address::FromMC(S07_FIRM_3),     &eeprom[_128K * 3],             _128K},
	{Address::FromMC(S08),            &eeprom[_128K * 4],             _128K},
	{Address::FromMC(S09),            &eeprom[_128K * 5],             _128K},
	{Address::FromMC(S10_SETTINGS_1), &eeprom[_128K * 6],             _128K},
	{Address::FromMC(S11_SETTINGS_2), &eeprom[_128K * 7],             _128K},
	{Address::FromMC(S12),            &eeprom[_128K * 8],             _16K},
	{Address::FromMC(S13),            &eeprom[_128K * 8 + _16K],      _16K},
	{Address::FromMC(S14),            &eeprom[_128K * 8 + _16K * 2],  _16K},
	{Address::FromMC(S15),            &eeprom[_128K * 8 + _16K * 3],  _16K},
	{Address::FromMC(S16),            &eeprom[_128K * 8 + _16K * 4],  _64K},
	{Address::FromMC(S17_RECORDER_1), &eeprom[_128K * 8 + _128K],     _128K},
	{Address::FromMC(S18_RECORDER_2), &eeprom[_128K * 8 + _128K * 2], _128K},
	{Address::FromMC(S19),            &eeprom[_128K * 8 + _128K * 3], _128K},
	{Address::FromMC(S20),            &eeprom[_128K * 8 + _128K * 4], _128K},
	{Address::FromMC(S21),            &eeprom[_128K * 8 + _128K * 5], _128K},
	{Address::FromMC(S22),            &eeprom[_128K * 8 + _128K * 6], _128K},
	{Address::FromMC(S23),            &eeprom[_128K * 8 + _128K * 7], _128K}
};


/// ¬озвращает ссылку на структуру описани€ сектора с начальным адресом address
static const SectorTypeDef &GetSector(Address address)
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
    const SectorTypeDef &sector = GetSector(Address::FromMC(address));

    std::memset(sector.memory, 0xFF, sector.size);
}


void HAL_EEPROM::WriteBytes(uint address, const uint8 *buffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        const SectorTypeDef &sector = GetSector(Address::FromMC(address));

        sector.WriteByte(Address::FromMC(address++), *buffer++);
    }
}