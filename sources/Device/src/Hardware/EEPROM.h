#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct DataSettings;

#define MAX_NUM_SAVED_WAVES 23  ///< \brief Число сохраняемых во внутреннем ППЗУ измерений. Пока ограничено количеством квадратиков, которые можно 
                                ///< вывести в одну линию внизу сетки.


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EEPROM
{
public:

    static void SaveSettings();

    static bool LoadSettings();

    static void DeleteAllData();

    static bool GetData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);

    static void DeleteData(int num);

    static void SaveData(int num, DataSettings *ds, uint8 *dataA, uint8 *dataB);
    /// Если даннные есть, соответствующий элемент массива равен true.
    static void GetDataInfo(bool existData[MAX_NUM_SAVED_WAVES]);

private:
    /// Возвращает адрес первого свободного байта в секторе настроек
    static uint FirstFreeAddressForSettings();
    /// \brief Возвращает адрес сохранённых настроек или 0, если настройки не сохранялись. fromEnd указывает, какие настройки от конца
    /// нужно загружать - 0 - последние, 1 - предпоследние и так далее
    static uint AddressSavedSettings(int fromEnd);
    /// Возвращает адрес первого свободного байта для записи
    static uint AddressFirstEmptyByte();

    static uint GetSector(uint address);

    static void EraseSector(uint address);

    static void WriteBytes(uint address, uint8 *data, int size);

    static void ReadBytes(uint address, void *data, uint size);

    static uint ReadDoubleWord(uint address);
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OTPmem
{
public:
    static bool SaveSerialNumber(char *servialNumber);
    /// Возвращает число свободных мест для записи. Если 0, то места в OTP уже не осталось.
    static int GetSerialNumber(char buffer[17]);
};

