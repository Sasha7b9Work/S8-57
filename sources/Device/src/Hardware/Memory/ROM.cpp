#include "defines.h"
#include "Hardware/Memory/ROM.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/Sector.h"


#define READ_BYTE(address)  (*((uint8 *)address))
#define SIZE_RECORD         512                      // Размер одной записи


class Record;

struct SectorSet
{
    // Возвращает адрес первой записи (фактически, начало сектора)
    Record *FirstRecord();

    // Возвращает указатель на последнюю запись с настройкаи
    Record *LastSaved();

    Sector sector;
};


static SectorSet sectorFirst = { Sector::Get(Sector::_10_SETTINGS_1) };
static SectorSet sectorSecond = { Sector::Get(Sector::_11_SETTINGS_2) };


/*
    В каждом секторе расположено (128 * 1024 / SIZE_RECORD) записей
*/

class Record
{
public:

    // Возвращает указатель на последнюю пустую запись (запись, в которую можно сохранить настройки)
    Record *NextEmpty();
    uint Number();
private:
    Settings set;
};


// Возвращает указатель на последнюю сохранённую запись
static Record *LastSaved();


void ROM::Settings::Save()
{
    Record *record = sectorFirst.FirstRecord();         // Адрес для записи настроек

    Record *last = LastSaved();                        // Указатель на последние сохранённые настройки

    if(last)
    {
        record = last->NextEmpty();                  // Находим адрес следующей за last записью в том же секторе

        if(!record)                                     // Если запись найти не удалось
        {

        }
    }

    set.number = record->Number() + 1;                // Если запись пустая, то номер будет равен 0 = 0xFFFFFFFF + 1
    set.crc32 = set.CalcWriteCRC32();
    HAL_ROM::WriteBufferBytes(reinterpret_cast<uint>(record), &set, sizeof(set));
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


bool ROM::Settings::Load()
{
    return false;
}


static Record *LastSaved()
{
    Record *saved1 = sectorFirst.LastSaved();
    Record *saved2 = sectorFirst.LastSaved();

    if (saved1 && saved2)
    {
        return (saved1->Number() > saved2->Number()) ? saved1 : saved2;
    }

    if (!saved1 && !saved2)
    {
        return nullptr;
    }

    if (!saved2)
    {
        return saved1;
    }

    return saved2;
}


uint Record::Number()
{
    return set.number;
}


Record *Record::NextEmpty()
{

}
