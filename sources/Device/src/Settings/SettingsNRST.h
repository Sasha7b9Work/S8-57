#pragma once


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
    operator uint8() const { return (uint8)value; };
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
    operator uint8() const { return (uint8)value; };
};


#define NRST_RSHIFT_ADD(ch, range, mode)    (set.nrst_rShiftAdd[ch][range][mode])
#define NRST_RSHIFT_ADD_A(range, mode)      (NRST_RSHIFT_ADD(Chan::A, range, mode))
#define NRST_RSHIFT_ADD_B(range, mode)      (NRST_RSHIFT_ADD(Chan::B, range, mode))

#define RSHIFT_ADD_STABLE(ch, range)        (set.nrst_rShiftAddStable[ch][range])
#define RSHIFT_ADD_STABLE_A(range)          (RSHIFT_ADD_STABLE(Chan::A, range))
#define RSHIFT_ADD_STABLE_B(range)          (RSHIFT_ADD_STABLE(Chan::B, range))

#define NRST_STRETCH_ADC_TYPE               (set.nrst_stretchADCtype)
#define NRST_STRETCH_ADC_TYPE_IS_DISABLE    (NRST_STRETCH_ADC_TYPE == StretchADC::Disable)
#define NRST_STRETCH_ADC_TYPE_IS_HAND       (NRST_STRETCH_ADC_TYPE == StretchADC::Hand)

#define NRST_STRETCH_ADC(ch, type)          (set.nrst_StretchADC[ch][type])
#define NRST_STRETCH_ADC_A(type)            (NRST_STRETCH_ADC(Chan::A, type))
#define NRST_STRETCH_ADC_B(type)            (NRST_STRETCH_ADC(Chan::B, type))

#define NRST_BALANCE_ADC_TYPE               (set.nrst_balanceADCtype)
#define NRST_BALANCE_ADC_TYPE_IS_HAND       (NRST_BALANCE_ADC_TYPE == BalanceADC::Hand)

#define NRST_BALANCE_ADC(ch)                (set.nrst_balanceADC[ch])
#define NRST_BALANCE_ADC_A                  (NRST_BALANCE_ADC(Chan::A))
#define NRST_BALANCE_ADC_B                  (NRST_BALANCE_ADC(Chan::B))

#define NRST_CORRECTION_TIME                (set.nrst_correctionTime)

#define NRST_ADD_STRETCH_20mV(ch)           (set.nrst_AddStretch20mV[ch])
#define NRST_ADD_STRETCH_20mV_A             (NRST_ADD_STRETCH_20mV(Chan::A))
#define NRST_ADD_STRETCH_20mV_B             (NRST_ADD_STRETCH_20mV(Chan::B))

#define NRST_ADD_STRETCH_50mV(ch)           (set.nrst_AddStretch50mV[ch])
#define NRST_ADD_STRETCH_50mV_A             (NRST_ADD_STRETCH_50mV(Chan::A))
#define NRST_ADD_STRETCH_50mV_B             (NRST_ADD_STRETCH_50mV(Chan::B))

#define NRST_ADD_STRETCH_100mV(ch)          (set.nrst_AddStretch100mV[ch])
#define NRST_ADD_STRETCH_100mV_A            (NRST_ADD_STRETCH_100mV(Chan::A))
#define NRST_ADD_STRETCH_100mV_B            (NRST_ADD_STRETCH_100mV(Chan::B))

#define NRST_ADD_STRETCH_2V(ch)             (set.nrst_AddStretch2V[ch])
#define NRST_ADD_STRETCH_2V_A               (NRST_ADD_STRETCH_2V(Chan::A))
#define NRST_ADD_STRETCH_2V_B               (NRST_ADD_STRETCH_2V(Chan::B))

#define NRST_NUM_SMOOTH_FOR_RAND            (set.nrst_numSmoothForRand)

#define NRST_NUM_AVE_FOR_RAND               (set.nrst_numAveForRand)
