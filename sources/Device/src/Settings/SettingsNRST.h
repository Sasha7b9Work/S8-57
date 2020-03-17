#pragma once
#include "Osci/ParametersOsci.h"


/// \brief Тип балансировки АЦП каналов.
/// Дело в том, что уровни АЦП не совпадают из-за отличия характеристик ( ? ), поэтому мы вводим дополнительное смещение для одного из АЦП канала.
struct BalanceADC
{
    enum E
    {
        Disabled,   ///< Балансировка выключена.
        Settings,   ///< Используются значения балансировки, которые получены автоматически.
        Hand        ///< Используются значения балансировки, заданные вручную.
    } value;
    static BalanceADC &Ref();
    static bool IsHand()            { return Ref().value == Hand; }
    static int16 &Value(Chan::E ch) { return Ref().balance[ch];   }
    int16  balance[Chan::Count];                    ///< Значение дополнительного смещения АЦП для ручной балансировки.
};

// Тип растяжки АЦП
struct StretchADC
{
    enum Type
    {
        Disabled,
        Real,
        Hand,
        Count
    } type;
    static void SetTypeDisabled();
    static void SetTypeReal();
    static bool TypeIsDisabled();
    static bool TypeIsHand();
    static float GetValue(Chan::E ch);
    static void SetValue(Chan::E ch, float value);
    float  value[Chan::Count];                                             // Хранится в целом виде, чтобы получить реальный коэффициент, нужно разделить на 1000 и прибавить единицу.
};


// Дополнительное смещение АЦП
struct ShiftADC
{
    enum E
    {
        Disabled,   // Дополнительное смещение не учитывется
        Real,       // Принимается дополнительное смещение, рассчитанное прик калибровке
        Count
    } type;
    static void SetTypeDisabled();
    static void SetTypeReal();
    static void SetValue(Chan::E ch, Range::E range, int8 value);
    static int8 GetValue(Chan::E ch, Range::E range);
    int8  value[Chan::Count][Range::Count];        // Добавочное смещение, которое пишется сюда при калибровке и балансировке
};


// Здесь хранятся настройки, которые определяются при калибровке и не перезаписываются при выключении прибора

struct SettingsNRST
{
    /*
        ВНИМАНИЕ !!! Для корректной работы нельзя уменьшать размер этой структуры
    */

    uint           size;                           // Размер структуры. За одно при загрузке из памяти можно определить, что структура пуста - здесь будет значение 0xFFFFFFFF
    int8           balanceShiftADC[Chan::Count];   // Добавочное смещение для балансировки АЦП
    int16          numAveForRand;                  // По скольким измерениям усреднять сигнал в режиме рандомизатора.
    int16          numSmoothForRand;               // Число точек для скользящего фильта в рандомизаторе.
    int16          correctionTime;                 // Коэффициент коррекции времени.
    int16          enum_gate_max;                  // Ограничение ворот в рандомизаторе сверху
    int16          enum_gate_min;                  // Ограничение ворот в рандомизаторе снизу
    BalanceADC     balanceADC;                     // Тип балансировки.
    ShiftADC       shiftADC;                       // Тип учитываемого при установке дополнительного смещения
    StretchADC     stretchADC;                     // Тип растяжки канала.
    // Инициализация при включении. Проиходит определние наличия настроек в ROM и их загрузка в случае, если настройки есть. Если настроек нету - инициализация значениями по умолчанию
    void Init();
    // Сохранение настроек в ROM. Происходит только если настройки в setNRST не совпадают с хранящимися в ROM
    void Save();

    bool operator!=(const SettingsNRST &rhs);
};


extern SettingsNRST setNRST;
