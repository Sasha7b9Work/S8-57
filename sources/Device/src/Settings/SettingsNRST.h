#pragma once
#include "Osci/Osci_Settings.h"


/// \brief Тип балансировки АЦП каналов.
/// Дело в том, что уровни АЦП не совпадают из-за отличия характеристик ( ? ), поэтому мы вводим дополнительное смещение для одного из АЦП канала.
struct BalanceADC
{
    enum E
    {
        Disable,     ///< Балансировка выключена.
        Settings,    ///< Используются значения балансировки, которые получены автоматически.
        Hand         ///< Используются значения балансировки, заданные вручную.
    } value;
    explicit BalanceADC(E v) : value(v) {};
};

/// Тип растяжки АЦП
struct StretchADC
{
    enum E
    {
        Disable,
        Real,
        Hand
    } value;
    explicit StretchADC(E v) : value(v) {};
};


#define NRST_CORRECTION_TIME                (set.nrst.correctionTime)
#define NRST_NUM_SMOOTH_FOR_RAND            (set.nrst.numSmoothForRand)
#define NRST_NUM_AVE_FOR_RAND               (set.nrst.numAveForRand)

#define NRST_RSHIFT_ADD(ch, range, mode)    (set.nrst.rShiftAdd[ch][range][mode])
#define NRST_RSHIFT_ADD_A(range, mode)      (NRST_RSHIFT_ADD(Chan::A, range, mode))
#define NRST_RSHIFT_ADD_B(range, mode)      (NRST_RSHIFT_ADD(Chan::B, range, mode))

#define RSHIFT_ADD_STABLE(ch, range)        (set.nrst.rShiftAddStable[ch][range])
#define RSHIFT_ADD_STABLE_A(range)          (RSHIFT_ADD_STABLE(Chan::A, range))
#define RSHIFT_ADD_STABLE_B(range)          (RSHIFT_ADD_STABLE(Chan::B, range))

#define NRST_STRETCH_ADC_TYPE               (set.nrst.stretchADCtype)
#define NRST_STRETCH_ADC_TYPE_IS_DISABLE    (NRST_STRETCH_ADC_TYPE == StretchADC::Disable)
#define NRST_STRETCH_ADC_TYPE_IS_HAND       (NRST_STRETCH_ADC_TYPE == StretchADC::Hand)

#define NRST_STRETCH_ADC(ch, type)          (set.nrst.StretchADC[ch][type])
#define NRST_STRETCH_ADC_A(type)            (NRST_STRETCH_ADC(Chan::A, type))
#define NRST_STRETCH_ADC_B(type)            (NRST_STRETCH_ADC(Chan::B, type))

#define NRST_BALANCE_ADC_TYPE               (set.nrst.balanceADCtype)
#define NRST_BALANCE_ADC_TYPE_IS_HAND       (NRST_BALANCE_ADC_TYPE == BalanceADC::Hand)

#define NRST_BALANCE_ADC(ch)                (set.nrst.balanceADC[ch])
#define NRST_BALANCE_ADC_A                  (NRST_BALANCE_ADC(Chan::A))
#define NRST_BALANCE_ADC_B                  (NRST_BALANCE_ADC(Chan::B))


struct SettingsNRST
{
    int16           balanceADC[Chan::Size];                 ///< Значение дополнительного смещения АЦП для ручной балансировки.
    BalanceADC::E   balanceADCtype;                         ///< Тип балансировки.
    StretchADC::E   stretchADCtype;                         ///< Тип растяжки канала.
    int16           StretchADC[Chan::Size][3];              ///< \brief Поправочный коэффициент для ручного, калибровочного и
                    ///< отключенного режимов. Здесь хранится в целом виде, чтобы получить реальный коэффициент, нужно разделить на 1000 и
                    ///< прибавить единицу.
    int16           rShiftAddStable[Chan::Size][3];         ///< Добавочное смещение для трёх самых чувствительных диапазонов. Задаётся единожды при настройке
    int16           numAveForRand;                          ///< По скольким измерениям усреднять сигнал в режиме рандомизатора.
    int16           numSmoothForRand;                       ///< Число точек для скользящего фильта в рандомизаторе.
    int16           rShiftAdd[Chan::Size][Range::Size][2];  ///< Добавочное смещение, которое пишется сюда при калибровке и балансировке
    int16           correctionTime;                         ///< Коэффициент коррекции времени.
};
