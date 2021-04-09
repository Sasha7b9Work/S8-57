#pragma once
#include "Utils/String.h"


struct DataSettings;
struct SettingsNRST;


struct ROM
{
	static const uint SIZE = 2 * 1024 * 1024;

    static void Init();

    // Функция для сохранения/восстановления настроек
    struct Settings
    {
        // Проверяет память и устраняет найденные ошибки
        static void CheckMemory();

        static void Save();

        static bool Load();

        static void Load(uint address);

        static void Erase();
    };

    // Функции для сохранения/загрузки несбрасываемых калибровочных настроек
    struct NRST
    {
        // Возвращает указатель на хранящиеся в ROM несбрасываемые настройки. nullptr в случае, если настроек там нет
        static SettingsNRST *GetSaved();
        
        // Сохраняет настройки из адреса nrst в ROM
        static void Save(SettingsNRST *nrst);

        static void Erase();
    };

    // Функция для сохранения/восстановления данных
    struct Data
    {
        static const uint MAX_NUM_SAVED_WAVES = 23; // \brief Число сохраняемых во внутреннем ППЗУ измерений. Пока ограничено количеством квадратиков, которые можно 
                                                    // вывести в одну линию внизу сетки.

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

