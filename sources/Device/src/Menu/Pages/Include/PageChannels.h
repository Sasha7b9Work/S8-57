#pragma once
#include "Menu/MenuItems.h"
#include "FPGA/FPGA_Settings.h"


#define SET_ENABLED_BOTH        (set.ch[Chan::A].enabled && set.ch[Chan::B].enabled)
#define SET_DISABLED_BOTH       (!set.ch[Chan::A].enabled && !set.ch[Chan::B].enabled)

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
{ //-V802
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
