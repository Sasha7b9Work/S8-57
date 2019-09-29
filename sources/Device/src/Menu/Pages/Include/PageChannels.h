#pragma once
#include "Menu/MenuItems.h"
#include "FPGA/FPGA_Settings.h"


#define SET_COUPLE(c)           (set.ch[c].couple)
#define SET_COUPLE_A            (SET_COUPLE(Chan::A))
#define SET_COUPLE_B            (SET_COUPLE(Chan::B))

#define SET_ENABLED_BOTH        (set.ch[Chan::A].enabled && set.ch[Chan::B].enabled)
#define SET_DISABLED_BOTH       (!set.ch[Chan::A].enabled && !set.ch[Chan::B].enabled)

#define SET_BALANCE_ADC(c)      (set.ch[c].balanceShiftADC)
#define SET_BALANCE_ADC_A       (SET_BALANCE_ADC(Chan::A))
#define SET_BALANCE_ADC_B       (SET_BALANCE_ADC(Chan::B))

#define SET_BANDWIDTH(c)        (set.ch[c].bandwidth)
#define SET_BANDWIDTH_A         (SET_BANDWIDTH(Chan::A))
#define SET_BANDWIDTH_B         (SET_BANDWIDTH(Chan::B))

#define SET_RESISTANCE(c)       (set.ch.resistance[c])
#define SET_RESISTANCE_A        (SET_RESISTANCE(Chan::A))
#define SET_RESISTANCE_B        (SET_RESISTANCE(Chan::B))

#define SET_CALIBR_MODE(c)      (set.ch.calibrationMode[c])
#define SET_CALIBR_MODE_A       (SET_CALIBR_MODE(Chan::A))
#define CALIBR_MODE_B           (SET_CALIBR_MODE(Chan::B))

#define DIVIDER_ABS(c)          ((set.ch[c].divider == Divider::_1) ? 1 : 10)

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
    uint16              rShift;              ///< Сдвиг канала по вертикали
    Range::E            range;               ///< Масштаб канала по вертикали
    ModeCouple::E       couple;              ///< Связь по входу
    bool                enabled;             ///< Включен/выключен канал
    int8                balanceShiftADC;     ///< Добавочное смещение для балансировки АЦП
    Bandwidth           bandwidth;           ///< Ограничение полосы
    bool                inverse;
    Divider::E          divider;             ///< Множитель
    CalibrationMode::E  calibrationMode;     ///< Режим калибровки
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
