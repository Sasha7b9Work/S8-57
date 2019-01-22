#include "defines.h"
#include "EEPROM.h"
#include "Hardware.h"
#include "Hardware/Sound.h"
#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Возвращает число свободных мест для записи. Если 0, то места в OTP уже не осталось.
static int GetSerialNumber(char buffer[17]);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Первый сектор для сохранения настроек. При его заполнении начинает использоваться сектор 2.
#define ADDR_SECTOR_SETTINGS_1  ((uint)0x080C0000)
/// Второй сектор для сохранения настроек. При его заполнении начинает использоваться сектор 1.
#define ADDR_SECTOR_SETTINGS_2  ((uint)0x080E0000)

#define SIZE_SECTOR_128         (128 * 1024)

#define CLEAR_FLASH_FLAGS                                                                   \
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP     |  /* end of operation flag              */   \
                            FLASH_FLAG_OPERR  |  /* operation error flag               */   \
                            FLASH_FLAG_WRPERR |  /* write protected error flag         */   \
                            FLASH_FLAG_PGAERR |  /* programming alignment error flag   */   \
                            FLASH_FLAG_PGPERR |  /* programming parallelism error flag */   \
                            FLASH_FLAG_PGSERR);  /* programming sequence error flag    */


#define READ_BYTE(address)          (*((uint8 *)address))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EEPROM::SaveSettings()
{
    /*
        Алгоритм нахождения первого свободного слова
        1. текущий адрес = (uint*)ADDR_SECTOR_SETTINGS_1.
        2. Смотрим, что записано по текущему адресу
        2. Если там 0xffffffff, то это первое свободное слово - выходим.
        3. Прибавляем к текущему адресу значение *((uint*)ADDR_SECTOR_SETTINGS_1).
        4. Переходим к 2
    */
    /*
        1. Записываем в Settings.size значение sizeof(Settings)
        2. Находим адрес первого свободного байта (алгоритм выше)
        3. Находим размер_свободной_памяти = ADDR_SECTOR_SETTINGS_1 + (128 * 1024) - адрес_первого_свободного_слова
        4. Если размер_свободной_памяти > размер_структуры_настроек, то сохраняем по найденному адресу структуру Settings и выходим.
        5. Иначе стираем сектор настроек и записываем в его начало структуру Settings
    */

    /// \todo Одного сектора для хранения настроек недостаточно, потому что после стирания может отключиться питание и тогда сотрутся все настройки

    set.size = sizeof(set);

    uint address = FirstFreeAddressForSettings();

    uint freeMemory = ADDR_SECTOR_SETTINGS_1 + SIZE_SECTOR_128 - address;

    if(address == MAX_UINT || freeMemory <= sizeof(Settings))
    {
        EraseSector(ADDR_SECTOR_SETTINGS_1);
        address = ADDR_SECTOR_SETTINGS_1;
    }

    set.size = sizeof(set);
    WriteBytes(address, (uint8 *)&set, sizeof(set));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint EEPROM::FirstFreeAddressForSettings() //-V2506
{
    uint address = ADDR_SECTOR_SETTINGS_1;

    do
    {
        uint value = ReadDoubleWord(address);

        if (value == MAX_UINT)              // Это условие означает, что по этому адресу ещё ничего не записывалось, иначе здесь был бы записан
        {                                   // размер структуры (Settings), чьим первым байтом являлось бы это слово
            return address;
        }

        address += value;                   // Переходим на первый свободный байт за структурой

    } while(address < (ADDR_SECTOR_SETTINGS_1 + SIZE_SECTOR_128));
    
    
    return MAX_UINT;        // Вообще-то до этой точки дойти никак не может. Если мы оказались здесь, произошла ошибка
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint EEPROM::AddressSavedSettings(int)
{
    uint addrPrev = 0;

    uint address = ADDR_SECTOR_SETTINGS_1;

    while (ReadDoubleWord(address) != MAX_UINT)
    {
        addrPrev = address;
        address += ReadDoubleWord(address);
    }

    return addrPrev;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint EEPROM::AddressFirstEmptyByte()
{
    uint address = AddressSavedSettings(0);
    return address + ReadDoubleWord(address);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint EEPROM::GetSector(uint address) //-V2506
{
    if (address == ADDR_SECTOR_SETTINGS_1)
    {
        return FLASH_SECTOR_10;
    }
    else if (address == ADDR_SECTOR_SETTINGS_2)
    {
        return FLASH_SECTOR_11;
    }
    else
    {
        // ничего не делаем
    }


    return FLASH_SECTOR_0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EEPROM::EraseSector(uint address)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef isFLASH =
    {
        FLASH_TYPEERASE_SECTORS,
        0,
        GetSector(address),
        1,
        FLASH_VOLTAGE_RANGE_3
    };

    uint error = 0;

    if (HAL_FLASHEx_Erase(&isFLASH, &error) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    HAL_FLASH_Lock();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EEPROM::WriteBytes(uint address, const uint8 *data, int size)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();
    
    for (int i = 0; i < size; i++)
    {
        if (HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, data[i]) != HAL_OK)
        {
            ERROR_HANDLER();
        }
        ++address;
    }

    HAL_FLASH_Lock();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EEPROM::ReadBytes(uint address, void *data, uint size)
{
    uint8 *buffer = (uint8 *)data;

    for (uint i = 0; i < size; i++)
    {
        buffer[i] = READ_BYTE(address++);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool EEPROM::GetData(int /*num*/, DataSettings * /*ds*/, uint8 * /*dataA*/, uint8 * /*dataB*/)
{
    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EEPROM::DeleteData(int /*num*/)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EEPROM::GetDataInfo(bool /*existData*/[MAX_NUM_SAVED_WAVES])
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EEPROM::SaveData(int /*num*/, DataSettings * /*ds*/, uint8 * /*dataA*/, uint8 * /*dataB*/)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EEPROM::DeleteAllData()
{
    /*
    EraseSector(ADDR_DATA_DATA);
    EraseSector(ADDR_DATA_0);
    EraseSector(ADDR_DATA_1);
    EraseSector(ADDR_DATA_2);
    EraseSector(ADDR_DATA_3);
    EraseSector(ADDR_DATA_4);
    EraseSector(ADDR_DATA_5);
    EraseSector(ADDR_DATA_TEMP);
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int GetSerialNumber(char buffer[17]) //-V2506
{
    /// \todo улучшить - нельзя разбрасываться байтами. Каждая запись должна занимать столько места, сколько в ней символов, а не 16, как сейчас.

    const int allShotsMAX = 512 / 16;   // Максимальное число записей в OTP серийного номера.

    uint8 *address = (uint8 *)FLASH_OTP_END - 15; //-V566

    do
    {
        address -= 16;
    } while (*address == 0xff && address > (uint8 *)FLASH_OTP_BASE); //-V566

    if (*address == 0xff)   // Не нашли строки с информацией, дойдя до начального адреса OTP
    {
        buffer[0] = 0;
        return allShotsMAX;
    }

    std::strcpy(buffer, (char *)address); //-V2513

    return allShotsMAX - (address - (uint8 *)FLASH_OTP_BASE) / 16 - 1; //-V566
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String OTPmem::GetSerialNumber(int *freeForWrite)
{
    char buffer[20];
    *freeForWrite = ::GetSerialNumber(buffer);
    return String(buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteBufferBytes(uint address, void *buffer, int size)
{
    Sound::WaitForCompletion();

    CLEAR_FLASH_FLAGS

        HAL_FLASH_Unlock();
    for (int i = 0; i < size; i++)
    {
        uint64_t data = ((uint8 *)buffer)[i];
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, data);
        address++;
    }
    HAL_FLASH_Lock();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool OTPmem::SaveSerialNumber(char *servialNumber) //-V2506
{
    // Находим первую пустую строку длиной 16 байт в области OTP, начиная с начала
    uint8 *address = (uint8 *)FLASH_OTP_BASE; //-V566

    while ((*address) != 0xff &&                    // *address != 0xff означает, что запись в эту строку уже производилась
           address < (uint8 *)FLASH_OTP_END - 16) //-V566
    {
        address += 16;
    }

    if (address < (uint8 *)FLASH_OTP_END - 16) //-V566
    {
        WriteBufferBytes((uint)address, (uint8 *)servialNumber, (int)std::strlen(servialNumber) + 1);
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint EEPROM::ReadDoubleWord(uint address)
{
    return (*((uint *)address));
}

