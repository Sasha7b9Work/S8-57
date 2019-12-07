#pragma once
#include "Settings/Settings.h"


struct TypeWave
{
    enum E
    {
        Sine,
        Meandr,
        Triangle
    } value;
};


struct Beeper
{
    static void Init();
    /// Звук нажатия на кнопку.
    static void ButtonPress();
    /// Функция вызовет звук отпускаемой кнопки только если перед этим проигрывался звук нажатия кнопки.
    static void ButtonRelease();

    static void GovernorChangedValue();
    
    static void RegulatorShiftRotate();

    static void RegulatorSwitchRotate();

    static void WarnBeepBad();

    static void WarnBeepGood();
    /// Эту функцию надо вызывать перед записью/стиранием ППЗУ. Звук конфликтует с ППЗУ.
    static void WaitForCompletion();

    static void Beep(TypePress::E type);

    static void Test();

    static uint8 Volume();

    struct Bell
    {
        static void On();

        static void Off();
    };
};
