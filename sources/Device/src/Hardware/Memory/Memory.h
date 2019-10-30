#pragma once
#include "Utils/String.h"


struct DataSettings;


#define MAX_NUM_SAVED_WAVES 23  ///< \brief Число сохраняемых во внутреннем ППЗУ измерений. Пока ограничено количеством квадратиков, которые можно 
                                ///< вывести в одну линию внизу сетки.


struct FlashMemory
{
	static const uint SIZE = 2 * 1024 * 1024;

    static void Write(uint address, const void *data, int size);

    /// Функция для сохранения/восстановления настроек
    struct Settings
    {
        static void Save();

        static bool Load();
    };

    /// Функция для сохранения/восстановления данных
    struct Data
    {
        /// Получает данные о сигнале номер num. Если сигнала нет, в ds записывается 0
        static bool Get(int num, DataSettings **ds, uint8 **dataA, uint8 **dataB);

        static void Delete(int num);

        static void DeleteAll();

        static void Save(int num, const DataSettings *ds, uint8 *dataA, uint8 *dataB);
        /// Если даннные есть, соответствующий элемент массива равен true.
        static void GetInfo(bool existData[MAX_NUM_SAVED_WAVES]);
    };
};



class OTPmem
{
public:
    static bool SaveSerialNumber(char *servialNumber);
    /// Возвращает текстовое представление серийного номера. В freeForWrite количество свободных мест для записи
    static String GetSerialNumber(int *freeForWrite);
};

