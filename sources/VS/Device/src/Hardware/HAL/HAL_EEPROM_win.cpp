#include "defines.h"
#include "Hardware/Memory/Memory.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


#define  _16K (16 * 1024)
#define  _64K (64 * 1024)
#define _128K (128 * 1024)


uint8 eeprom[FlashMemory::SIZE];


const uint HAL_FLASH::Sector::address[SEC_NUM_SECTORS] =
{
    reinterpret_cast<uint>(&eeprom[0]),                     // 00_BOOT_1
    reinterpret_cast<uint>(&eeprom[_16K]),                  // 01_BOOT_2
    reinterpret_cast<uint>(&eeprom[_16K * 2]),              // 02
    reinterpret_cast<uint>(&eeprom[_16K * 3]),              // 03
    reinterpret_cast<uint>(&eeprom[_16K * 4]),              // 04
    reinterpret_cast<uint>(&eeprom[_128K]),                 // 05_FIRM_1
    reinterpret_cast<uint>(&eeprom[_128K * 2]),             // 06_FIRM_2
    reinterpret_cast<uint>(&eeprom[_128K * 3]),             // 07_FIRM_3
    reinterpret_cast<uint>(&eeprom[_128K * 4]),             // 08
    reinterpret_cast<uint>(&eeprom[_128K * 5]),             // 09
    reinterpret_cast<uint>(&eeprom[_128K * 6]),             // 10_SETTINGS_1
    reinterpret_cast<uint>(&eeprom[_128K * 7]),             // 11_SETTINGS_2
    reinterpret_cast<uint>(&eeprom[_128K * 8]),             // 12
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K]),      // 13
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K * 2]),  // 14
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K * 3]),  // 15
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _16K * 4]),  // 16
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K]),     // 17_RECORDER_1
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 2]), // 18_RECORDER_2
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 3]), // 19_DATA_1
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 4]), // 20_DATA_2
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 5]), // 21_DATA_3
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 6]), // 22_DATA_4
    reinterpret_cast<uint>(&eeprom[_128K * 8 + _128K * 7])  // 23_DATA_5
};

const uint HAL_FLASH::Sector::size[SEC_NUM_SECTORS] =
{
    _16K,   // 00_BOOT_1
    _16K,   // 01_BOOT_2
    _16K,   // 02
    _16K,   // 03
    _64K,   // 04
    _128K,  // 05_FIRM_1
    _128K,  // 06_FIRM_2
    _128K,  // 07_FIRM_3
    _128K,  // 08
    _128K,  // 09
    _128K,  // 10_SETTINGS_1
    _128K,  // 11_SETTINGS_2
    _16K,   // 12
    _16K,   // 13
    _16K,   // 14
    _16K,   // 15
    _64K,   // 16
    _128K,  // 17_RECORDER_1
    _128K,  // 18_RECORDER_2
    _128K,  // 19_DATA_1
    _128K,  // 20_DATA_2
    _128K,  // 21_DATA_3
    _128K,  // 22_DATA_4
    _128K   // 23_DATA_5
};


void HAL_FLASH::WriteBufferBytes(uint address, const void *buffer, int size)
{
    WriteBytes(address, static_cast<const uint8 *>(buffer), size);
}


void HAL_FLASH::WriteBytes(uint address, const uint8 *buffer, int size)
{
    for (int i = 0; i < size; i++)
    {
        *reinterpret_cast<uint8 *>(address) = *buffer;
        address++;
        buffer++;
    }
}

int HAL_FLASH::Sector::Num(uint startAddress)
{
    for (int i = 0; i < SEC_NUM_SECTORS; i++)
    {
        if (startAddress < address[i] + size[i])
        {
            return i;
        }
    }

    return -1;
}

void HAL_FLASH::Sector::Erase(int num)
{
    std::memset(reinterpret_cast<void *>(address[num]), 0xFF, HAL_FLASH::Sector::size[num]);
}
