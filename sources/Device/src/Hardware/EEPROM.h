#pragma once
#include "Utils/String.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct DataSettings;

#define MAX_NUM_SAVED_WAVES 23  ///< \brief Число сохраняемых во внутреннем ППЗУ измерений. Пока ограничено количеством квадратиков, которые можно 
                                ///< вывести в одну линию внизу сетки.


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class OTPmem
{
public:
    static bool SaveSerialNumber(char *servialNumber);
    /// Возвращает текстовое представление серийного номера. В freeForWrite количество свободных мест для записи
    static String GetSerialNumber(int *freeForWrite);
};

