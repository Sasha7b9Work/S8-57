#include "defines.h"
#include "Hardware/Memory/Memory.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"


#define  _16K (16 * 1024)
#define  _64K (64 * 1024)
#define _128K (128 * 1024)


uint8 eeprom[FlashMemory::SIZE];


const Sector HAL_FLASH::sectors[Sector::Count] =
{
    { Sector::_00_BOOT_1,     reinterpret_cast<uint>(&eeprom[0]),                     _16K },
    { Sector::_01_BOOT_2,     reinterpret_cast<uint>(&eeprom[_16K]),                  _16K },
    { Sector::_02,            reinterpret_cast<uint>(&eeprom[_16K * 2]),              _16K },
    { Sector::_03,            reinterpret_cast<uint>(&eeprom[_16K * 3]),              _16K },
    { Sector::_04,            reinterpret_cast<uint>(&eeprom[_16K * 4]),              _64K },
    { Sector::_05_FIRM_1,     reinterpret_cast<uint>(&eeprom[_128K]),                 _128K },
    { Sector::_06_FIRM_2,     reinterpret_cast<uint>(&eeprom[_128K * 2]),             _128K },
    { Sector::_07_FIRM_3,     reinterpret_cast<uint>(&eeprom[_128K * 3]),             _128K },
    { Sector::_08,            reinterpret_cast<uint>(&eeprom[_128K * 4]),             _128K },
    { Sector::_09,            reinterpret_cast<uint>(&eeprom[_128K * 5]),             _128K },
    { Sector::_10_SETTINGS_1, reinterpret_cast<uint>(&eeprom[_128K * 6]),             _128K },
    { Sector::_11_SETTINGS_2, reinterpret_cast<uint>(&eeprom[_128K * 7]),             _128K },
    { Sector::_12,            reinterpret_cast<uint>(&eeprom[_128K * 8]),             _16K },
    { Sector::_13,            reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K]),      _16K },
    { Sector::_14,            reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K * 2]),  _16K },
    { Sector::_15,            reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K * 3]),  _16K },
    { Sector::_16,            reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K * 4]),  _64K },
    { Sector::_17_RECORDER_1, reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K]),     _128K },
    { Sector::_18_RECORDER_2, reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 2]), _128K },
    { Sector::_19_DATA_1,     reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 3]), _128K },
    { Sector::_20_DATA_2,     reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 4]), _128K },
    { Sector::_21_DATA_3,     reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 5]), _128K },
    { Sector::_22_DATA_4,     reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 6]), _128K },
    { Sector::_23_DATA_5,     reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 7]), _128K }
};


void HAL_FLASH::WriteBufferBytes(uint address, const void *buffer, uint size)
{
    WriteBytes(address, static_cast<const uint8 *>(buffer), size);
}


void HAL_FLASH::WriteBytes(uint address, const uint8 *buffer, uint size)
{
    for (uint i = 0; i < size; i++)
    {
        *reinterpret_cast<uint8 *>(address) = *buffer;
        address++;
        buffer++;
    }
}
