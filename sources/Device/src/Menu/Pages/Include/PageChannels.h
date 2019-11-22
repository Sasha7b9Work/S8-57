#pragma once
#include "Menu/MenuItems.h"
#include "Settings/SettingsOsci.h"


#define SET_ENABLED_BOTH        (set.ch[Chan::A].enabled && set.ch[Chan::B].enabled)
#define SET_DISABLED_BOTH       (!set.ch[Chan::A].enabled && !set.ch[Chan::B].enabled)


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
    Divider(Chan::E ch);
    int ToAbs() const;
    static int ToAbs(Divider::E v) { return (v == _1) ? 1 : 10; };
    operator int() { return value; }
};


struct SettingsChannel
{ //-V802
    int16               rShift;              ///< Сдвиг канала по вертикали
    Range::E            range;               ///< Масштаб канала по вертикали
    ModeCouple::E       couple;              ///< Связь по входу
    bool                enabled;             ///< Включен/выключен канал
    int8                balanceShiftADC;     ///< Добавочное смещение для балансировки АЦП
    Bandwidth           bandwidth;           ///< Ограничение полосы
    bool                inverse;
    Divider::E          divider;             ///< Множитель
    CalibrationMode::E  calibrationMode;     ///< Режим калибровки
};



struct PageChannelA
{
    static void OnChanged_Couple(bool active);

    static const Page * const self;
};



struct PageChannelB
{
    static void OnChanged_Couple(bool active);

    static const Page * const self;
};
