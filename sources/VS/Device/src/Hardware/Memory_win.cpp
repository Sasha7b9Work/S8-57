#include "defines.h"
#include "Hardware/Memory.h"


#define  _16K (16 * 1024)
#define  _64K (64 * 1024)
#define _128K (128 * 1024)


uint8 eeprom[2 * 1024 * 1024];

static const SectorTypeDef sectors[24] =
{
    {0x08000000, &eeprom[0],                     _16K},     // sector0
    {0x08004000, &eeprom[_16K],                  _16K},     // sector1
    {0x08008000, &eeprom[_16K * 2],              _16K},     // sector2
    {0x0800C000, &eeprom[_16K * 3],              _16K},     // sector3
    {0x08010000, &eeprom[_16K * 4],              _64K},     // sector4
    {0x08020000, &eeprom[_128K],                 _128K},    // sector5
    {0x08040000, &eeprom[_128K * 2],             _128K},    // sector6
    {0x08060000, &eeprom[_128K * 3],             _128K},    // sector7
    {0x08080000, &eeprom[_128K * 4],             _128K},    // sector8
    {0x080A0000, &eeprom[_128K * 5],             _128K},    // sector9
    {0x080C0000, &eeprom[_128K * 6],             _128K},    // sector10
    {0x080E0000, &eeprom[_128K * 7],             _128K},    // sector11
    {0x08100000, &eeprom[_128K * 8],             _16K},     // sector12
    {0x08104000, &eeprom[_128K * 8 + _16K],      _16K},     // sector13
    {0x08108000, &eeprom[_128K * 8 + _16K * 2],  _16K},     // sector14
    {0x0810C000, &eeprom[_128K * 8 + _16K * 3],  _16K},     // sector15
    {0x08110000, &eeprom[_128K * 8 + _16K * 4],  _64K},     // sector16
    {0x08120000, &eeprom[_128K * 8 + _128K],     _128K},    // sector17
    {0x08140000, &eeprom[_128K * 8 + _128K * 2], _128K},    // sector18
    {0x08160000, &eeprom[_128K * 8 + _128K * 3], _128K},    // sector19
    {0x08180000, &eeprom[_128K * 8 + _128K * 4], _128K},    // sector20
    {0x081A0000, &eeprom[_128K * 8 + _128K * 5], _128K},    // sector21
    {0x081C0000, &eeprom[_128K * 8 + _128K * 6], _128K},    // sector22
    {0x081E0000, &eeprom[_128K * 8 + _128K * 7], _128K}     // sector23
};


bool SectorTypeDef::IsConsist(uint addr) const
{
    return (addr >= (uint)memory) && (addr < (uint)(memory + size));
}


void SectorTypeDef::WriteByte(uint addr, uint8 value) const
{
    if (IsConsist(addr))
    {
        *((uint8 *)addr) = value;
    }
}
