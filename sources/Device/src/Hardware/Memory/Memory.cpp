#include "defines.h"
#include "Memory.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"


/// ¬озвращает адрес первого свободного байта в секторе настроек
static uint FirstFreeAddressForSettings();
/// \brief ¬озвращает адрес сохранЄнных настроек или 0, если настройки не сохран€лись. fromEnd указывает, какие настройки от конца
/// нужно загружать - 0 - последние, 1 - предпоследние и так далее
static uint AddressSavedSettings(int fromEnd);

static void ReadBytes(uint address, void *data, uint size);

static uint ReadDoubleWord(uint address);



#define SIZE_SECTOR_128         (128 * 1024)

#define READ_BYTE(address)      (*((uint8 *)address))



void FlashMemory::SaveSettings()
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

    uint freeMemory = S10_SETTINGS_1 + SIZE_SECTOR_128 - address;

    if((address == MAX_UINT) || (freeMemory <= sizeof(Settings)) || (address < S10_SETTINGS_1))
    {
        EraseSector(S10_SETTINGS_1);
        address = S10_SETTINGS_1;
    }

    set.size = sizeof(set);

    WriteData(address, &set, sizeof(set));
}


void FlashMemory::EraseSector(uint address)
{
    HAL_EEPROM::EraseSector(address);
}


void FlashMemory::WriteData(uint address, const void *data, int size)
{
    HAL_EEPROM::WriteBytes(address, static_cast<const uint8 *>(data), size);
}


static uint FirstFreeAddressForSettings() //-V2506
{
    uint address = S10_SETTINGS_1;

    do
    {
        uint value = ReadDoubleWord(address);

        if (value == MAX_UINT)              // Ёто условие означает, что по этому адресу ещЄ ничего не записывалось, иначе здесь был бы записан
        {                                   // размер структуры (Settings), чьим первым байтом €вл€лось бы это слово
            return address;
        }

        address += value;                   // ѕереходим на первый свободный байт за структурой

    } while(address < (S10_SETTINGS_1 + SIZE_SECTOR_128));
    
    
    return MAX_UINT;        // ¬ообще-то до этой точки дойти никак не может. ≈сли мы оказались здесь, произошла ошибка
}


static uint AddressSavedSettings(int)
{
    uint addrPrev = 0;

    uint address = S10_SETTINGS_1;

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


bool FlashMemory::Data::Get(int /*num*/, DataSettings * /*ds*/, uint8 * /*dataA*/, uint8 * /*dataB*/)
{
    return false;
}


void FlashMemory::Data::Delete(int /*num*/)
{
}


void FlashMemory::Data::GetInfo(bool /*existData*/[MAX_NUM_SAVED_WAVES])
{
}


void FlashMemory::Data::Save(int /*num*/, const DataSettings * /*ds*/, uint8 * /*dataA*/, uint8 * /*dataB*/)
{
}


void FlashMemory::DeleteAllData()
{
}


String OTPmem::GetSerialNumber(int *freeForWrite)
{
    char buffer[20];
    *freeForWrite = HAL_OTP::GetSerialNumber(buffer);
    return String(buffer);
}


bool OTPmem::SaveSerialNumber(char *servialNumber)
{
    return HAL_OTP::SaveSerialNumber(servialNumber);
}


static uint ReadDoubleWord(uint address)
{
    return *(reinterpret_cast<uint *>(address));
}


bool FlashMemory::LoadSettings()
{
    uint address = AddressSavedSettings(0);

    if (address && ReadDoubleWord(address) == sizeof(set))
    {
        ReadBytes(address, &set, ReadDoubleWord(address));
        return true;
    }

    return false;
}
