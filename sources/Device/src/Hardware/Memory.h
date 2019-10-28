#pragma once
#include "Utils/String.h"


#ifdef GUI

struct SectorTypeDef
{
    uint address;
    uint8 *memory;
    uint size;
    /// Возвращает true, если сектор содержит ячейку памяти с адресом addr
    bool IsConsist(uint addr) const;

    void WriteByte(uint addr, uint8 value) const;
};

extern const SectorTypeDef sectors[24];

#define ADDR_SECTOR_SETTINGS_1 ((uint)sectors[10].memory)
#define ADDR_SECTOR_RECORDER_1 ((uint)sectors[17].memory)

#endif

#define S00            ((uint)0x08000000)
#define S01            ((uint)0x08004000)
#define S02            ((uint)0x08008000)
#define S03            ((uint)0x0800C000)
#define S04            ((uint)0x08010000)
#define S05            ((uint)0x08020000)
#define S06            ((uint)0x08040000)
#define S07            ((uint)0x08060000)
#define S08            ((uint)0x08080000)
#define S09            ((uint)0x080A0000)
/// Первый сектор для сохранения настроек. При его заполнении начинает использоваться сектор 2.
#define S10_SETTINGS_1 ((uint)0x080C0000)
#define S11            ((uint)0x080E0000)
#define S12            ((uint)0x08100000)
#define S13            ((uint)0x08104000)
#define S14            ((uint)0x08108000)
#define S15            ((uint)0x0810C000)
#define S16            ((uint)0x08110000)
#define S17_RECORDER_1 ((uint)0x08120000)
#define S18_RECORDER_2 ((uint)0x08140000)
#define S19            ((uint)0x08160000)
#define S20            ((uint)0x08180000)
#define S21            ((uint)0x081A0000)
#define S22            ((uint)0x081C0000)
#define S23            ((uint)0x081E0000)


struct DataSettings;

#define MAX_NUM_SAVED_WAVES 23  ///< \brief Число сохраняемых во внутреннем ППЗУ измерений. Пока ограничено количеством квадратиков, которые можно 
                                ///< вывести в одну линию внизу сетки.


struct EEPROM
{
    static void SaveSettings();

    static bool LoadSettings();

    static void DeleteAllData();
    /// Стирает сектор с начальным адресом address
    static void EraseSector(uint address);

    static void WriteData(uint address, const void *data, int size);

    static bool GetData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);

    static void DeleteData(int num);

    static void SaveData(int num, const DataSettings *ds, uint8 *dataA, uint8 *dataB);
    /// Если даннные есть, соответствующий элемент массива равен true.
    static void GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES]);
};



class OTPmem
{
public:
    static bool SaveSerialNumber(char *servialNumber);
    /// Возвращает текстовое представление серийного номера. В freeForWrite количество свободных мест для записи
    static String GetSerialNumber(int *freeForWrite);
};

