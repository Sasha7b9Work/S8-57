#pragma once
#include "Utils/String.h"


#define SEC_00_BOOT_1     ((uint)0x08000000)   //  16k
#define SEC_01_BOOT_2     ((uint)0x08004000)   //  16k
#define SEC_02            ((uint)0x08008000)   //  16k
#define SEC_03            ((uint)0x0800C000)   //  16k
#define SEC_04            ((uint)0x08010000)   //  64k
#define SEC_05_FIRM_1     ((uint)0x08020000)   // 128k
#define SEC_06_FIRM_2     ((uint)0x08040000)   // 128k
#define SEC_07_FIRM_3     ((uint)0x08060000)   // 128k
#define SEC_08            ((uint)0x08080000)   // 128k
#define SEC_09            ((uint)0x080A0000)   // 128k
#define SEC_10_SETTINGS_1 ((uint)0x080C0000)   // 128k
#define SEC_11_SETTINGS_2 ((uint)0x080E0000)   // 128k
#define SEC_12            ((uint)0x08100000)   //  16k
#define SEC_13            ((uint)0x08104000)   //  16k
#define SEC_14            ((uint)0x08108000)   //  16k
#define SEC_15            ((uint)0x0810C000)   //  16k
#define SEC_16            ((uint)0x08110000)   //  64k
#define SEC_17_RECORDER_1 ((uint)0x08120000)   // 128k
#define SEC_18_RECORDER_2 ((uint)0x08140000)   // 128k
#define SEC_19_DATA_1     ((uint)0x08160000)   // 128k
#define SEC_20_DATA_2     ((uint)0x08180000)   // 128k
#define SEC_21_DATA_3     ((uint)0x081A0000)   // 128k
#define SEC_22_DATA_4     ((uint)0x081C0000)   // 128k
#define SEC_23_DATA_5     ((uint)0x081E0000)   // 128k


struct DataSettings;

#define MAX_NUM_SAVED_WAVES 23  ///< \brief Число сохраняемых во внутреннем ППЗУ измерений. Пока ограничено количеством квадратиков, которые можно 
                                ///< вывести в одну линию внизу сетки.


/// Структура описывает адрес в адресном пространстве контроллера
struct Address
{
    uint addressMC;
    /// Возвращает адрес из адресного пространства контроллера, рассчитанный из адреса PC
    static Address FromPC(uint addr);
    static Address FromPC(uint8 *addr);
    /// Формируте адрес из адресного пространства микроконтроллера
    static Address FromMC(uint addr);
    /// Преобразует в адрес PC
    uint8 *ToPC() const;
private:
    explicit Address(uint addr) : addressMC(addr) {};
};

struct FlashMemory
{
	static const uint SIZE = 2 * 1024 * 1024;

    /// Стирает сектор с начальным адресом address
    static void EraseSector(uint address);

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

