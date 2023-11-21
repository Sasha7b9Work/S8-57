// 2023/11/21 10:04:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Keyboard.h"


struct TypeWave
{
    enum E
    {
        Sine,
        Meandr,
        Triangle
    } value;
};


namespace Beeper
{
    void Init();

    // Звук нажатия на кнопку.
    void ButtonPress();

    // Функция вызовет звук отпускаемой кнопки только если перед этим проигрывался звук нажатия кнопки.
    void ButtonRelease();

    void GovernorChangedValue();

    void RegulatorShiftRotate();

    void RegulatorSwitchRotate();

    void WarnBeepBad();

    void WarnBeepGood();

    // Эту функцию надо вызывать перед записью/стиранием ППЗУ. Звук конфликтует с ППЗУ.
    void WaitForCompletion();

    void Beep(TypePress::E type);

    void Test();

    namespace Bell
    {
        void On();

        void Off();
    };
};
