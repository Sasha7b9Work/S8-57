#pragma once
#include "Menu/MenuItems.h"
#include "FPGA/FPGA_Settings.h"


#define SET_INVERSE(c)          (set.ch.inverse[c])
#define SET_INVERSE_A           (SET_INVERSE(Chan::A))
#define SET_INVERSE_B           (SET_INVERSE(Chan::B))
#define SET_RSHIFT_MATH         (set.math.rShift)
#define SET_RANGE_MATH          (set.math.range)

#define SET_RANGE(c)            (set.ch.range[c])
#define SET_RANGE_A             (SET_RANGE(Chan::A))
#define SET_RANGE_B             (SET_RANGE(Chan::B))

#define SET_COUPLE(c)           (set.ch.couple[c])
#define SET_COUPLE_A            (SET_COUPLE(Chan::A))
#define SET_COUPLE_B            (SET_COUPLE(Chan::B))

#define SET_ENABLED(c)          (set.ch.enabled[c])
#define SET_ENABLED_A           (SET_ENABLED(Chan::A))
#define SET_ENABLED_B           (SET_ENABLED(Chan::B))
#define SET_ENABLED_BOTH        (SET_ENABLED_A && SET_ENABLED_B)
#define SET_DISABLED_BOTH       (!SET_ENABLED_A && !SET_ENABLED_B)

#define SET_BALANCE_ADC(c)      (set.ch.balanceShiftADC[c])
#define SET_BALANCE_ADC_A       (SET_BALANCE_ADC(Chan::A))
#define SET_BALANCE_ADC_B       (SET_BALANCE_ADC(Chan::B))

#define SET_BANDWIDTH(c)        (set.ch.bandwidth[c])
#define SET_BANDWIDTH_A         (SET_BANDWIDTH(Chan::A))
#define SET_BANDWIDTH_B         (SET_BANDWIDTH(Chan::B))

#define SET_RESISTANCE(c)       (set.ch.resistance[c])
#define SET_RESISTANCE_A        (SET_RESISTANCE(Chan::A))
#define SET_RESISTANCE_B        (SET_RESISTANCE(Chan::B))

#define SET_CALIBR_MODE(c)      (set.ch.calibrationMode[c])
#define SET_CALIBR_MODE_A       (SET_CALIBR_MODE(Chan::A))
#define CALIBR_MODE_B           (SET_CALIBR_MODE(Chan::B))

#define DIVIDER_ABS(c)          ((set.ch.divider[c] == Divider::_1) ? 1 : 10)

/// Режим калибровки.
struct CalibrationMode
{
    enum E
    {
        x1,
        x10,
        Disable
    } value;
};

/// Делитель.
struct Divider
{
    enum E
    {
        _1,
        _10
    } value;
    explicit Divider(E v) : value(v) { };
    explicit Divider(uint v) : value(static_cast<E>(v)) { };
    int ToAbs() const { return (value == _1) ? 1 : 10; };
};


struct SettingsChannel
{
    uint16                         shift[Chan::Size];      ///< Сдвиг канала по вертикали
    Range::E       range[Chan::Size];      ///< Масштаб канала по вертикали
    FPGA::Settings::ModeCouple::E  couple[Chan::Size];     ///< Связь по входу
    bool                           enabled[Chan::Size];    ///< Включен/выключен канал
    int8                           balanceShiftADC[2];     ///< Добавочное смещение для балансировки АЦП
    FPGA::Settings::Bandwidth      bandwidth[2];           ///< Ограничение полосы
    bool                           inverse[2];
    Divider::E                     divider[2];             ///< Множитель
    CalibrationMode::E             calibrationMode[2];     ///< Режим калибровки
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageChannelA
{
    static void OnChanged_Input(bool active);

    static void OnChanged_Couple(bool active);

    static const Page * const self;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageChannelB
{
    static void OnChanged_Input(bool active);

    static void OnChanged_Couple(bool active);

    static const Page * const self;
};
