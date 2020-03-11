#pragma once
#include "Utils/String.h"


struct DataSettings;
struct SettingsNRST;


struct ROM
{
	static const uint SIZE = 2 * 1024 * 1024;

    // Функция для сохранения/восстановления настроек
    struct Settings
    {
        static void Save();

        static bool Load();

        // Возвращает адрес первого свободного байта в секторе настроек
        static uint FirstFreeAddressForSettings();

        // \brief Возвращает адрес сохранённых настроек или 0, если настройки не сохранялись. fromEnd указывает, какие настройки от конца
        // нужно загружать - 0 - последние, 1 - предпоследние и так далее
        static uint AddressSavedSettings(int fromEnd);

        static void ReadBytes(uint address, void *data, uint size);

        static uint ReadDoubleWord(uint address);
    };

    // Функции для сохранения/загрузки несбрасываемых калибровочных настроек
    struct NRST
    {
        // Возвращает указатель на хранящиеся в ROM несбрасываемые настройки. nullptr в случае, если настроек там нет
        static SettingsNRST *GetSaved();
        // Сохраняет настройки из адреса nrst в ROM
        static void Save(SettingsNRST *nrst);
    };

    // Функция для сохранения/восстановления данных
    struct Data
    {
        static const uint MAX_NUM_SAVED_WAVES = 23; ///< \brief Число сохраняемых во внутреннем ППЗУ измерений. Пока ограничено количеством квадратиков, которые можно 
                                                    ///< вывести в одну линию внизу сетки.

        // Если даннные есть, соответствующий элемент массива равен true.
        static void GetInfo(bool existData[MAX_NUM_SAVED_WAVES]);

        static void Save(uint numInROM, const DataSettings *ds);
        
        // Получает данные о сигнале номер num. Если сигнала нет, в ds записывается 0
        static const DataSettings *Read(uint numInROM);

        static void Erase(uint numInROM);

        static void EraseAll();
    };
};



struct OTP
{
    static bool SaveSerialNumber(char *servialNumber);
    
    // Возвращает текстовое представление серийного номера. В freeForWrite количество свободных мест для записи
    static String GetSerialNumber(int *freeForWrite);
};

