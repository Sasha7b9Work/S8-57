#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


struct SectorTypeDef
{
    uint address;
    uint8 *memory;
    uint size;

    /// ¬озвращает true, если сектор содержит €чейку пам€ти с адресом addr
    bool IsConsist(uint addr) const
    {
        return (addr >= address) && (addr < (address + size));
    }
    void WriteByte(uint addr, uint8 value) const
    {
        if (IsConsist(addr))
        {
            memory[addr - address] = value;
        }
    }
};

#define  _16K (16 * 1024)
#define  _64K (64 * 1024)
#define _128K (128 * 1024)

static uint8 memory[2 * 1024 * 1024];


static const SectorTypeDef sectors[24] =
{
    {0x08000000, &memory[0],                     _16K},     // sector0
    {0x08004000, &memory[_16K],                  _16K},     // sector1
    {0x08008000, &memory[_16K * 2],              _16K},     // sector2
    {0x0800C000, &memory[_16K * 3],              _16K},     // sector3
    {0x08010000, &memory[_16K * 4],              _64K},     // sector4
    {0x08020000, &memory[_128K],                 _128K},    // sector5
    {0x08040000, &memory[_128K * 2],             _128K},    // sector6
    {0x08060000, &memory[_128K * 3],             _128K},    // sector7
    {0x08080000, &memory[_128K * 4],             _128K},    // sector8
    {0x080A0000, &memory[_128K * 5],             _128K},    // sector9
    {0x080C0000, &memory[_128K * 6],             _128K},    // sector10
    {0x080E0000, &memory[_128K * 7],             _128K},    // sector11
    {0x08100000, &memory[_128K * 8],             _16K},     // sector12
    {0x08104000, &memory[_128K * 8 + _16K],      _16K},     // sector13
    {0x08108000, &memory[_128K * 8 + _16K * 2],  _16K},     // sector14
    {0x0810C000, &memory[_128K * 8 + _16K * 3],  _16K},     // sector15
    {0x08110000, &memory[_128K * 8 + _16K * 4],  _64K},     // sector16
    {0x08120000, &memory[_128K * 8 + _128K],     _128K},    // sector17
    {0x08140000, &memory[_128K * 8 + _128K * 2], _128K},    // sector18
    {0x08160000, &memory[_128K * 8 + _128K * 3], _128K},    // sector19
    {0x08180000, &memory[_128K * 8 + _128K * 4], _128K},    // sector20
    {0x081A0000, &memory[_128K * 8 + _128K * 5], _128K},    // sector21
    {0x081C0000, &memory[_128K * 8 + _128K * 6], _128K},    // sector22
    {0x081E0000, &memory[_128K * 8 + _128K * 7], _128K}     // sector23
};


/// ¬озвращает ссылку на структуру описани€ сектора с начальным адресом address
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