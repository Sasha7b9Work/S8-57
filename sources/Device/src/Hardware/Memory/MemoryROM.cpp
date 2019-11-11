#include "defines.h"
#include "Hardware/Memory/MemoryROM.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"


/// ¬озвращает адрес первого свободного байта в секторе настроек
static uint FirstFreeAddressForSettings();
/// \brief ¬озвращает адрес сохранЄнных настроек или 0, если настройки не сохран€лись. fromEnd указывает, какие настройки от конца
/// нужно загружать - 0 - последние, 1 - предпоследние и так далее
static uint AddressSavedSettings(int fromEnd);

static void ReadBytes(uint address, void *data, uint size);

static uint ReadDoubleWord(uint address);


#define READ_BYTE(address)      (*((uint8 *)address))



void ROM::Settings::Save()
{
    /*
        јлгоритм нахождени€ первого свободного слова
        1. текущий адрес = (uint*)ADDR_SECTOR_SETTINGS_1.
        2. —мотрим, что записано по текущему адресу
        2. ≈сли там 0xffffffff, то это первое свободное слово - выходим.
        3. ѕрибавл€ем к текущему адресу значение *((uint*)ADDR_SECTOR_SETTINGS_1).
        4. ѕереходим к 2
    */
    /*
        1. «аписываем в Settings.size значение sizeof(Settings)
        2. Ќаходим адрес первого свободного байта (алгоритм выше)
        3. Ќаходим размер_свободной_пам€ти = ADDR_SECTOR_SETTINGS_1 + (128 * 1024) - адрес_первого_свободного_слова
        4. ≈сли размер_свободной_пам€ти > размер_структуры_настроек, то сохран€ем по найденному адресу структуру Settings и выходим.
        5. »наче стираем сектор настроек и записываем в его начало структуру Settings
    */

    /// \todo ќдного сектора дл€ хранени€ настроек недостаточно, потому что после стирани€ может отключитьс€ питание и тогда сотрутс€ все настройки

    set.size = sizeof(set);

    uint address = FirstFreeAddressForSettings();

    uint freeMemory = END_SECTOR(Sector::_10_SETTINGS_1) - address;

    if((address == MAX_UINT) || (freeMemory <= sizeof(Settings)) || (address < ADDR_SECTOR(Sector::_10_SETTINGS_1)))
    {
        SECTOR(Sector::_10_SETTINGS_1).Erase();

        address = ADDR_SECTOR(Sector::_10_SETTINGS_1);
    }

    set.size = sizeof(set);

    HAL_ROM::WriteBufferBytes(address, &set, sizeof(set));
}


static uint FirstFreeAddressForSettings() //-V2506
{
    uint address = ADDR_SECTOR(Sector::_10_SETTINGS_1);

    do
    {
        uint value = ReadDoubleWord(address);

        if (value == MAX_UINT)              // Ёто условие означает, что по этому адресу ещЄ ничего не записывалось, иначе здесь был бы записан
        {                                   // размер структуры (Settings), чьим первым байтом €вл€лось бы это слово
            return address;
        }

        address += value;                   // ѕереходим на первый свободный байт за структурой

    } while (address < END_SECTOR(Sector::_10_SETTINGS_1));
    
    return MAX_UINT;        // ¬ообще-то до этой точки дойти никак не может. ≈сли мы оказались здесь, произошла ошибка
}


static uint AddressSavedSettings(int)
{
    uint addrPrev = 0;

    uint address = ADDR_SECTOR(Sector::_10_SETTINGS_1);

    while (ReadDoubleWord(address) != MAX_UINT)
    {
        addrPrev = address;
        address += ReadDoubleWord(address);
    }

    return addrPrev;
}


static void ReadBytes(uint address, void *data, uint size)
{
    uint8 *buffer = static_cast<uint8 *>(data);

    for (uint i = 0; i < size; i++)
    {
        buffer[i] = READ_BYTE(address++);
    }
}

String OTP::GetSerialNumber(int *freeForWrite)
{
    char buffer[20];
    *freeForWrite = HAL_OTP::GetSerialNumber(buffer);
    return String(buffer);
}


bool OTP::SaveSerialNumber(char *servialNumber)
{
    return HAL_OTP::SaveSerialNumber(servialNumber);
}


static uint ReadDoubleWord(uint address)
{
    return *(reinterpret_cast<uint *>(address));
}


bool ROM::Settings::Load()
{
    uint address = AddressSavedSettings(0);

    if (address && ReadDoubleWord(address) == sizeof(set))
    {
        ReadBytes(address, &set, ReadDoubleWord(address));
        return true;
    }

    return false;
}
