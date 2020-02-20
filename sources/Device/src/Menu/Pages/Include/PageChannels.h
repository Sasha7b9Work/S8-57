#pragma once
#include "Menu/MenuItems.h"
#include "Settings/ParametersOsci.h"


/// Делитель.
struct Divider
{
    enum E
    {
        _1,
        _10
    };
    Divider(Chan::E _ch) : ch(_ch) {};
    static Divider::E &Ref(Chan::E);
    operator Divider::E()          { return Ref(ch); };
    int ToAbs()                    { return Is1() ? 1 : 10; }
    static int ToAbs(Divider::E v) { return (v == _1) ? 1 : 10; };
    bool Is1()                     { return Ref(ch) == _1; }
private:
    Chan::E ch;
};


struct SettingsChannel
{ //-V802
    int16               rShift;              ///< Сдвиг канала по вертикали
    Range::E            range;               ///< Масштаб канала по вертикали
    ModeCouple::E       couple;              ///< Связь по входу
    bool                enabled;             ///< Включен/выключен канал
    Bandwidth::E        bandwidth;           ///< Ограничение полосы
    bool                inverse;
    Divider::E          divider;             ///< Множитель
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
