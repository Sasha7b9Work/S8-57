#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


#define  _16K (16 * 1024)
#define  _64K (64 * 1024)
#define _128K (128 * 1024)


static uint8 sector0 [ _16K];   // 0x08000000
static uint8 sector1 [ _16K];   // 0x08004000
static uint8 sector2 [ _16K];   // 0x08008000
static uint8 sector3 [ _16K];   // 0x0800C000
static uint8 sector4 [ _64K];   // 0x08010000
static uint8 sector5 [_128K];   // 0x08020000
static uint8 sector6 [_128K];   // 0x08040000
static uint8 sector7 [_128K];   // 0x08060000
static uint8 sector8 [_128K];   // 0x08080000
static uint8 sector9 [_128K];   // 0x080A0000
static uint8 sector10[_128K];   // 0x080C0000
static uint8 sector11[_128K];   // 0x080E0000
static uint8 sector12[ _16K];   // 0x08100000
static uint8 sector13[ _16K];   // 0x08104000
static uint8 sector14[ _16K];   // 0x08108000
static uint8 sector15[ _16K];   // 0x0810C000
static uint8 sector16[ _64K];   // 0x08110000
static uint8 sector17[_128K];   // 0x08120000
static uint8 sector18[_128K];   // 0x08140000
static uint8 sector19[_128K];   // 0x08160000
static uint8 sector20[_128K];   // 0x08180000
static uint8 sector21[_128K];   // 0x081A0000
static uint8 sector22[_128K];   // 0x081C0000
static uint8 sector23[_128K];   // 0x081E0000


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


static const SectorTypeDef sectors[24] =
{
    {0x08000000, sector0,   _16K},
    {0x08004000, sector1,   _16K},
    {0x08008000, sector2,   _16K},
    {0x0800C000, sector3,   _16K},
    {0x08010000, sector4,   _64K},
    {0x08020000, sector5,  _128K},
    {0x08040000, sector6,  _128K},
    {0x08060000, sector7,  _128K},
    {0x08080000, sector8,  _128K},
    {0x080A0000, sector9,  _128K},
    {0x080C0000, sector10, _128K},
    {0x080E0000, sector11, _128K},
    {0x08100000, sector12,  _16K},
    {0x08104000, sector13,  _16K},
    {0x08108000, sector14,  _16K},
    {0x0810C000, sector15,  _64K},
    {0x08110000, sector16, _128K},
    {0x08120000, sector17, _128K},
    {0x08140000, sector18, _128K},
    {0x08160000, sector19, _128K},
    {0x08180000, sector20, _128K},
    {0x081A0000, sector21, _128K},
    {0x081C0000, sector22, _128K},
    {0x081E0000, sector23, _128K}
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