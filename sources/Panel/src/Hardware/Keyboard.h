// 2023/11/21 10:38:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Controls.h"
#include "defines.h"


struct Keyboard
{
    static void Init();

    static void Update();

    //  оличество выводов RL дл€ опроса клавиатуры
    static const int NUM_RL = 6;

    //  оличество выводов SL дл€ опроса клавиатуры
    static const int NUM_SL = 8;

    // ¬озращает им€ органа управлени€
    static const char *ControlName(Control control);

    static bool *GetStateControls();
};
