#pragma once
#include "Menu/MenuItems.h"


// Делитель.
struct Divider
{
    enum E
    {
        _1,
        _10
    };

    static int ToAbs(Divider::E v) { return (v == _1) ? 1 : 10; };
};


struct SettingsChannel
{ //-V802
    int16               rShift;              // Сдвиг канала по вертикали
    Range::E            range;               // Масштаб канала по вертикали
    ModeCouple::E       couple;              // Связь по входу
    bool                enabled;             // Включен/выключен канал
    Bandwidth::E        bandwidth;           // Ограничение полосы
    bool                inverse;
    Divider::E          divider;             // Множитель
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
